#include "SiTCP.h"
#include <sys/select.h>
#include <netinet/in.h>
#include <TTimeStamp.h>
#include "PacketDecoder.h"
using namespace std;
ClassImp(SiTCP);

SiTCP::SiTCP(){
    setFileMaxSize();
    setSocketBufferSize();
    setDir();
    setDataProcessHost();
    rate = 0;
    nTasks = 0;
    maxFileID = std::numeric_limits<uint64_t>::max();
    firstFile = true;
    writeBuffer = false;
    cout<<"initsitcp\n";
}
SiTCP::~SiTCP(){
}
void SiTCP::setDataProcessHost(int port, const char* host){
    dataPort = port;
    dataHost = host;
}
void SiTCP::setDir(const char* _dir){
    dir = _dir;
    if(dir[dir.length()-1]!='/')dir.append("/");
    std::filesystem::create_directory(dir.c_str());
    clearDir();
}
void SiTCP::createFile(){
    file.open((dir+"writing.buffer").c_str(), std::ios::binary);
}
void SiTCP::closeFile(){
    file.close();
    rename ( (dir+"writing.buffer").c_str(), ( dir+to_string(daqFileID)+".a").c_str() );
    // cout<<"DAQ file "<<( dir+to_string(daqFileID)+".a")<<" generated"<<endl;
    daqFileID++;
    if(daqFileID == maxFileID) daqFileID = 0;
}
void SiTCP::clearDir(){
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".buffer";
        if(name.size()>7){
            if(name.substr(name.size() - suffix.size()) == suffix) std::filesystem::remove(file.path());
        }
        name = file.path().filename().string();
        suffix = name.substr(name.size() - 2);
        if( suffix == ".a") std::filesystem::remove(file.path());
        if( suffix == ".b") std::filesystem::remove(file.path());
        if( suffix == ".c") std::filesystem::remove(file.path());
    }
    daqFileID = 0;
    decFileID = 0;
}
void SiTCP::createSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}
void SiTCP::setupServerAddress(const char* ip, int port){
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);
}
void SiTCP::connectDevice(){
    if(connectionStatus == connected)return;
    createSocket();
    if(connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        connectionStatus = disconnected;
    }else{ 
        connectionStatus = connected; 
    }
}
void SiTCP::disconnectDevice(){
    if(connectionStatus == disconnected) return;
    close(sock);
    connectionStatus = disconnected;
}
void SiTCP::stop(){
    status = status_stopping;
    DAQThread->join();
    DecodeThread->join();
}
void SiTCP::run(){
    status = status_running;
    DAQThread = new thread(&SiTCP::DAQLoop, this);
    DecodeThread = new thread(&SiTCP::DecodeLoop, this);
}
// bool SiTCP::sendToDevice(const char* msg){
//     lock.lock();
//     disconnectDevice();
//     connectDevice();
//     if(connectionStatus == disconnected){
//         std::cout<<"Can Not Connection to SiTCP"<<endl;
//         lock.unlock();
//         return false;
//     }
//     int length = send(sock, msg, 9, 0);
//     disconnectDevice();
//     lock.unlock();
//     if(length == 9) return true;
//     return false;
// }
void SiTCP::setSocketBufferSize(int n){
    socketBufferSize = n;
    socketBuffer = new char[n];
}
void SiTCP::setFileMaxSize(int n){
    fileMaxSize = n;
}
void SiTCP::enableDecoder(){
    writeBuffer = true;
}
void SiTCP::disableDecoder(){
    writeBuffer = false;
}
void SiTCP::DAQLoop(){
    std::cout<<"data acquisition loop start"<<endl;
    int fileSize = 0;
    uint64_t dataSize = 0;
    int nDaq4Count = 1;
    int daqCount = 0;
    clearDir();
    int length;
    TTimeStamp start_time, now, elapsed;
    start_time.Set();
    while(status == status_running){
        connectDevice();
        if(connectionStatus == disconnected){
            std::cout<<"Can Not Connection to SiTCP"<<endl;
            continue;
        }
        length = recv(sock, socketBuffer, socketBufferSize, 0);
        if(length <=0 ){
            sleep(0.1);
            // cout<<"sitcp get data length "<<length<<endl;
            rate = length;
            continue;
        }
        daqCount++;
        dataSize += length;
        if(daqCount >= nDaq4Count){
            now.Set();
            elapsed = now - start_time;
            double t = static_cast<double>(elapsed.AsDouble());
            if(t == 0){
                nDaq4Count++;
            }else{
                rate = dataSize /(1024*1024 * t);//MB/s

                start_time.Set();
                dataSize = 0;
                daqCount = 0;
            }
        }
        if(!file.is_open()){
            if(writeBuffer)createFile();
        }
        if(file.is_open()){
            file.write(socketBuffer, length);
            fileSize += length;
            if(fileSize > fileMaxSize){
                closeFile();
                fileSize = 0;
            }
        }
    }
    if(file.is_open()) closeFile();
    disconnectDevice();
    cout<<"data acquisition loop stop"<<endl;
}

void SiTCP::DecodeLoop(){
    cout<<"data decoding loop start"<<endl;
    while(status == status_running){
        string name = dir+to_string(decFileID)+".a";
        if (std::filesystem::exists(name)){
            TSocket* rootsock;
            if(sockDeque.size()==0){
                rootsock = new TSocket(dataHost.c_str(), dataPort);
                if(rootsock->IsValid() == kFALSE){
                    continue;
                }   
            }else{
                rootsock = *sockDeque.begin();
                sockDeque.pop_front();
            }
            new thread(&SiTCP::DecodeTask, this, decFileID, rootsock);
            decFileID++;
            if(decFileID == maxFileID) decFileID = 0;
        }
    }
    cout<<"data decoding loop stop"<<endl;
}

void SiTCP::DecodeTask(int id, TSocket* rootsock){
    nTasks++;
    char byte;
    PacketDecoder decoder;
    ifstream file;
    string filename = dir+to_string(id)+".b";
    rename((dir+to_string(id)+".a").c_str(), filename.c_str());
    file.open(filename.c_str(), std::ios::binary);
    while(file.read(&byte,1) && (status == status_running)){
        int state = decoder.Fill(&byte);
        if(state >0) {
            // cout<<"decoder: event id "<<decoder.rawEvent.event_id<<endl;
            TMessage mess(kMESS_OBJECT);
            mess.WriteObject(&(decoder.rawEvent));
            while(!(rootsock->Send(mess))){
                if(status != status_running)break;
            }
            decoder.rawEvent.reset();
        }
    }
    file.close();
    if(id == 0 && firstFile) {
        std::filesystem::path filepath= filename.c_str();
        std::filesystem::remove(filepath);
        firstFile = false;
    }else{
        rename(filename.c_str(),(dir+to_string(id)+".c").c_str());
    }
    uint64_t nextID = id+1;
    if(nextID == maxFileID) nextID = 0;
    filename =  dir+to_string(nextID)+".c";
    while(!std::filesystem::exists(filename)){
        if(status != status_running) {
            nTasks--;
            if(rootsock->IsValid())sockDeque.push_back(rootsock);
            return;
        }
    }
    file.open(filename.c_str(), std::ios::binary);
    while(file.read(&byte,1) && (status == status_running)){
        int state = decoder.Fill(&byte);
        if(state >0) {
            TMessage mess(kMESS_OBJECT);
            mess.WriteObject(&(decoder.rawEvent));
            while(!(rootsock->Send(mess))){
                if(status != status_running)break;
            }
            decoder.rawEvent.reset();
            break ;
        }
        if(state <0) break;
    }
    file.close();
    std::filesystem::path filepath= filename.c_str();
    std::filesystem::remove(filepath);
    nTasks--;
    if(rootsock->IsValid())sockDeque.push_back(rootsock);
}