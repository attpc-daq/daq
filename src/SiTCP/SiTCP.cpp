#include "SiTCP.h"
#include <sys/select.h>
#include <netinet/in.h>

ClassImp(SiTCP);

SiTCP::SiTCP(){
    fileMaxSize = 1024*1024*1024;
    setSocketBufferSize(1024*1024);
    dir = "./output/";
    prefix = "packet";
    fileID = 0;
}
SiTCP::~SiTCP(){
}
void SiTCP::setFilePrefix(const char* _prefix){
    prefix = _prefix;
}
void SiTCP::setDir(const char* _dir){
    dir = _dir;
    if(dir[dir.length()-1]!='/')dir.append("/");
    std::filesystem::create_directory(dir.c_str());
}
void SiTCP::createFile(){
    file.open((dir+"writing.dat").c_str(), std::ios::binary);
}
void SiTCP::closeFile(){
    file.close();
    //updateFileID();
    rename ( (dir+"writing.dat").c_str(), ( dir+prefix+to_string(fileID)+".dat").c_str() );
    fileID++;
}
void SiTCP::updateFileID(){
    fileID = -1;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".dat";
        if(name.substr(0, prefix.size()) != prefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(prefix.size(),name.size() - suffix.size()));
        if(id>fileID)fileID = id;
    }
    fileID++;
}
void SiTCP::clearDir(){
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".dat";
        if(name.substr(0, prefix.size()) != prefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        std::filesystem::remove(file.path());
    }
    fileID = 0;
}
void SiTCP::createSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}
void SiTCP::connectToDevice(const char* ip, int port){
    createSocket();
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
    }
    FD_ZERO(&sockReadSet);
    FD_ZERO(&sockWriteSet);
    FD_SET(sock,&sockReadSet);
    FD_SET(sock,&sockWriteSet);
    max_fd = sock;
}
void SiTCP::stop(){
    status = status_stopping;
    DAQThread->join();
}
void SiTCP::run(){
    DAQThread = new thread(&SiTCP::DAQLoop, this);
}
void SiTCP::sendToDevice(const char* msg){
    int length;
    lock.lock();
    activity = select(max_fd+1, NULL, &sockWriteSet, NULL,NULL);
    if((activity<0) &&(errno != EINTR)){
            cout<<"SiTCP socket select error"<<endl;
    }
    if(FD_ISSET(sock,&sockWriteSet)){
        // lock.lock();
        send(sock, msg, 9, 0);
        std::cout << "send to device: 0x";
        for (int i = 0; i < 9; i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint>(msg[i]&0b0000000011111111);
        }
        std::cout<<std::dec << std::endl;
        // lock.unlock();
    }
    lock.unlock();
}
void SiTCP::setSocketBufferSize(int n){
    socketBufferSize = n;
    socketBuffer = new char[n];
}
void SiTCP::setFileMaxSize(int n){
    fileMaxSize = n;
}
void SiTCP::DAQLoop(){
    cout<<"data acquisition loop start"<<endl;
    connectionStatus = disconnected;
    status = status_running;
    int fileSize = 0;
    createFile();
    int length=0;
    while(status == status_running){
        lock.lock();
        activity = select(max_fd+1, &sockReadSet, NULL,NULL,NULL);
        if((activity<0) &&(errno != EINTR)){
            cout<<"SiTCP socket select error"<<endl;
        }
        if(FD_ISSET(sock,&sockReadSet)){
            // 
            length = recv(sock, socketBuffer, socketBufferSize, 0);
            // lock.unlock();
            //cout<<"sitcp get data "<<length<<endl;
        }
        lock.unlock();
        if(length <=0 ){
            sleep(0.1);
            cout<<"sitcp get data length "<<length<<endl;
            continue;
        }
        file.write(socketBuffer, length);
        fileSize += length;
        if(fileSize > fileMaxSize){
            closeFile();
            createFile();
            fileSize = 0;
        }
    }
    closeFile();
    close(sock);
    cout<<"data acquisition loop stop"<<endl;
}