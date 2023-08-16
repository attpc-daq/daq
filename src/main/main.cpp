#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int sock;
struct sockaddr_in server_address;
int connected = 1;
int disconnected = 0;
// int status;
int connectionStatus;
// int nTasks;

void createSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void setupServerAddress(const char* ip, int port){
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);
}

void connectDevice(){
    if(connectionStatus == connected)return;
    createSocket();
    if(connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        connectionStatus = disconnected;
    }else{ 
        connectionStatus = connected; 
    }
}

int main() {
    // createSocket();
    setupServerAddress("192.168.10.16",4660);
    connectDevice();
     char* socketBuffer = new char[1024*1024];
    while(true){
        if(connectionStatus == disconnected){
            std::cout<<"Can Not Connection to SiTCP"<<std::endl;
            // lock.unlock();
            continue;
        }
        int length = recv(sock, socketBuffer, 1024*1024, 0);
        std::cout<<"get data "<<length<<std::endl;

    }
    // TH1F *h1 = new TH1F("hpx","This is the px distribution",100,-4,4);
    // TMessage::EnableSchemaEvolutionForAll(false); 
    // TMessage mess(kMESS_OBJECT);
    // mess.WriteObject(h1);

    // cout<<"put "<<((RawEvent*)obj)->event_id<<endl;
    // cout<<"class"<<buffer[tail]->GetClass()<<endl;
    // std::cout<<"class "<<mess.GetClass()<<std::endl;
 

    return 0;
}
