#include "AutoSocket.h"

AutoSocket::AutoSocket(int _port) {
    // Constructor for a server, which is for sending data
    isSender = true;
    isReceiver = false;
    port = _port;
    buffer = NULL;
    bufferFile = NULL;
    ACKBuffer = new char[20];
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    // setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);
    // printf("Socket is bound to address %s and port %d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    sender_fd = -1;
    connection = false;
}

AutoSocket::AutoSocket(int _port, const char* _host) {
    // Constructor for a client, which is for receiving data
    isSender = false;
    isReceiver = true;
    port = _port;
    host = _host;
    if(host == "localhost")host="0.0.0.0";
    buffer = new char[1024 * 1024 * 20]; // buffer size should be large enough
    bufferFile = new TBufferFile(TBuffer::kRead);
    ACKBuffer = new char[20];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    connection = false;
}

AutoSocket::~AutoSocket() {
    if (server_fd != -1) close(server_fd);
    if (sender_fd != -1) close(sender_fd);
    if (receiver_fd != -1) close(receiver_fd);
    if (buffer != NULL) delete[] buffer;
    if (bufferFile != NULL) delete bufferFile;
}

TObject* AutoSocket::get(const TClass* cl) {
    // Return a TObject object when receiving data
    // Return NULL if no data is received
    if(!checkConnection()) return NULL;
    // if(debug)cout<<"AutoSocket::get, debug 1"<<endl;
    int n = read(receiver_fd, &size1, sizeof(size1));
    // if(debug)cout<<"AutoSocket::get, debug 1.5, n:"<<n<<endl;
    if(n == 0){ //disconnected
        disconnect();
        return NULL;
    }
    if(n < 0 ){ //timeout
        return NULL;
    }
    // if(debug)cout<<"AutoSocket::get, debug 2, size "<<size1<<endl;
    n = ::send(receiver_fd, &size1, sizeof(size1), 0);
    if(n < 0 ){
        disconnect();
        return NULL;
    }
    int total = 0;
    while(total<size1){
        int n = read(receiver_fd, buffer+total, size1);
        if(n == 0){
            disconnect();
            return NULL;
        }
        total += n;
    }
    // if(debug)cout<<"AutoSocket::get, debug 3"<<endl;
    n = ::send(receiver_fd, &total, sizeof(total), 0);
    if(n < 0 ){
        disconnect();
        return NULL;
    }
    if(total != size1){
        disconnect();
        return NULL;
    }
    // if(debug)cout<<"AutoSocket::get, debug 4"<<endl;
    bufferFile->Reset();
    bufferFile->SetBuffer(buffer, size1, kFALSE);
    return bufferFile->ReadObject(cl);
}
bool AutoSocket::send(RawEvent* rawEvent){
    TBufferFile* bufferFile = new TBufferFile(TBuffer::kWrite, 1024*1024*20);
    bufferFile->WriteObject(rawEvent);
    bool sendState = send(bufferFile);
    delete bufferFile;
    return sendState;
}
bool AutoSocket::send(TBufferFile* bf) {
    // if(debug)cout<<"PID:"<<getpid()<<"AutoSocket::send debug 1"<<endl;
    if(!checkConnection()) {
        return false;
    }
    int size1 = bf->Length();
    // if(debug)cout<<"PID:"<<getpid()<<"AutoSocket::send debug 2, size:"<<size1<<endl;
    int n = ::send(sender_fd, &size1, sizeof(size1), 0);
    if(n < 0 ){
        disconnect();
        return false;
    }
    // if(debug)cout<<"PID:"<<getpid()<<"AutoSocket::send debug 3"<<endl;
    n = read(sender_fd, &size2, sizeof(size2));
    if(n == 0 ){
        disconnect();
        return false;
    }
    if(n < 0 ){
        return false;
    }
    if(size1!=size2){
        disconnect();
        return false;
    }
    // if(debug)cout<<"PID:"<<getpid()<<"AutoSocket::send debug 4"<<endl;
    n = ::send(sender_fd, bf->Buffer(), size1, 0);
    if(n < 0 ){
        disconnect();
        return false;
    }
    // if(debug)cout<<"PID:"<<getpid()<<"AutoSocket::send debug 5"<<endl;
    n = read(sender_fd, &size3, sizeof(size3));
    if(n == 0 ){
        disconnect();
        return false;
    }
    if(n < 0 ){
        return false;
    }
    if(size1!=size3){
        disconnect();
        return false;
    }
    // if(debug)cout<<"PID:"<<getpid()<<"AutoSocket::send debug 6"<<endl;
    return true;
}
bool AutoSocket::checkConnection(){
    if(connection) return true;
    if(isSender){
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        sender_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (sender_fd < 0) {
            // perror("Accept");
            return false;
        }
    }else{
        receiver_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (receiver_fd < 0) {
            // perror("Socket creation failed");
            return false;
        }
        if(connect(receiver_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return false;
        }else{
            struct timeval tv;
            tv.tv_sec = 3;
            tv.tv_usec = 0;
            setsockopt(receiver_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        }
    }
    connection = true;
    return true;
}
void AutoSocket::disconnect(){
    if(isSender)close(sender_fd);
    if(isReceiver)close(receiver_fd);
    connection = false;
}