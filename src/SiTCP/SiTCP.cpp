#include "SiTCP.h"
#include <sys/select.h>
#include <netinet/in.h>
#include <TTimeStamp.h>
#include <TROOT.h>
#include "PacketDecoder.h"
using namespace std;

SiTCP::SiTCP(int id){
    ROOT::EnableThreadSafety();
    keyID = id;
    key_t shmkey = ftok(".", keyID);
    shmid = shmget(shmkey, sizeof(struct shmseg), 0644|IPC_CREAT);
    if (shmid == -1) {
      perror("SiTCP Shared memory error");
    }
    shmp = (struct shmseg*) (shmat(shmid, NULL, 0));
    if (shmp == (void *) -1) {
      perror("SiTP Shared memory attach error");
    } 
}
SiTCP::~SiTCP(){
    shmctl(shmid, IPC_RMID, NULL);
}
void SiTCP::resetSHM(){
    shmp->status = 0;
    shmp->nTasks = 0;
    shmp->rate =0;
    shmp->writeBuffer = false;
    shmp->dataAcquisitionStatus = 0;
    shmp->dataDecodStatus = 0;
    shmp->nQueues = 0;
}
void SiTCP::start(){
    if(shmp->status == status_not_started){
        shmp->status = status_starting;
        runPID = fork();
        if(runPID == 0){
            char *path = getenv("PATH");
            char  pathenv[strlen(path) + sizeof("PATH=")];
            sprintf(pathenv, "PATH=%s", path);
            char *envp[] = {pathenv, NULL};
            char *const args[] = {(char*)"SiTCPExec", (char*)to_string(keyID).c_str(), NULL};
            execvpe("SiTCPExec", args, envp);
        }
        while(shmp->status != status_running){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
void SiTCP::stop(){
    shmp->status = status_stopping;
    if(runPID >0){
        int status;
        waitpid(runPID, &status, 0);
        if(WIFEXITED(status)){
            // cout<<" SiTCP RUN process exited with status: "<< WEXITSTATUS(status) << std::endl;
        }else{
            std::cerr << "SiTCP RUN process terminated abnormally." << std::endl;
        }
    }
}
void SiTCP::run(){
    ROOT::EnableThreadSafety();
    shmp->status = status_running;
    thread * DAQThread=NULL;
    thread * DecodeThread=NULL;
    thread * DataSenderThread=NULL;
    LockFreeQueue<LockFreeQueue<TBufferFile*>*> *queue = new LockFreeQueue<LockFreeQueue<TBufferFile*>*>();
    while(shmp->status == status_running){
        if(DAQThread==NULL){
            if(shmp->dataAcquisitionStatus == status_starting){
                DAQThread = new thread(&SiTCP::DAQLoop, this);
            }
        }else{
            if(shmp->dataAcquisitionStatus >= status_stopping){
                DAQThread->join();
                delete DAQThread;
                DAQThread = NULL;
            }
        }
        if(DecodeThread==NULL){
            if(shmp->dataDecodStatus == status_starting){
                DecodeThread = new thread(&SiTCP::DecodeLoop, this, queue);
            }
        }else{
            if(shmp->dataDecodStatus >= status_stopping){
                DecodeThread->join();
                delete DecodeThread;
                DecodeThread = NULL;
            }
        }
        if(DataSenderThread==NULL){
            DataSenderThread = new thread(&SiTCP::DataSenderLoop, this, queue);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if(DAQThread!=NULL){
        shmp->dataAcquisitionStatus = status_stopping;
        DAQThread->join();
        delete DAQThread;
        DAQThread = NULL;
    }
    if(DecodeThread!=NULL){
        shmp->dataDecodStatus = status_stopping;
        DecodeThread->join();
        delete DecodeThread;
        DecodeThread = NULL;
    }
    if(DataSenderThread!=NULL){
        DataSenderThread->join();
        delete DataSenderThread;
        DataSenderThread = NULL;
    }
    delete queue;
    shmp->status = status_stopped;
}
void SiTCP::startData(){
    shmp->dataAcquisitionStatus = status_starting;
    while(shmp->dataAcquisitionStatus != status_running){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void SiTCP::stopData(){
    shmp->dataAcquisitionStatus = status_stopping;
    while(shmp->dataAcquisitionStatus != status_stopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void SiTCP::startDecoder(){
    shmp->dataDecodStatus = status_starting;
    shmp->writeBuffer = true;
    while(shmp->dataDecodStatus != status_running){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void SiTCP::stopDecoder(){
    shmp->writeBuffer = false;
    shmp->dataDecodStatus = status_stopping;
    while(shmp->dataDecodStatus != status_stopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
int SiTCP::getState(){
    return shmp->status;
}
float SiTCP::getRate(){
    return shmp->rate;
}
int SiTCP::getNTasks(){
    return shmp->nTasks;
}
int SiTCP::getNQueues(){
    return shmp->nQueues;
}
void SiTCP::setDataPort(int port){
    shmp->dataPort = port;
}
void SiTCP::setDir(const char* dir){
    strncpy(shmp->dir,dir,strlen(dir));
    if(shmp->dir[strlen(dir)-1]!='/')shmp->dir[strlen(dir)]='/';
    std::filesystem::create_directory(shmp->dir);
    // clearDir();
}
void SiTCP::createFile(){
    string dir=shmp->dir;
    file.open((dir+"writing.buffer").c_str(), std::ios::binary);
}
void SiTCP::closeFile(){
    file.close();
    string dir = shmp->dir;
    rename ( (dir+"writing.buffer").c_str(), ( dir+to_string(daqFileID)+".a").c_str() );
    daqFileID++;
    if(daqFileID == maxFileID) daqFileID = 0;
}
void SiTCP::clearDir(){
    for (const auto & file : std::filesystem::directory_iterator(shmp->dir)){
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

void SiTCP::setServerAddress(const char* ip, int port){
    shmp->FEEPort = port;
    strncpy(shmp->FEEAddressStr, ip, strlen(ip));
    shmp->FEEAddress = inet_addr(ip);
}
int SiTCP::connectDevice(){
    cout<<"connect to SiTCP"<<endl;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = shmp->FEEAddress;
    server_address.sin_port = htons(shmp->FEEPort);
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }
    //设置socket的timeout
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection to SiTCP failed");
        disconnectDevice();
        // close(sockfd);
        // FD_ZERO(&sockReadSet);
        return -1;
    }
    FD_SET(sockfd, &sockReadSet);
    cout<<"connect to SiTCP success"<<endl;
    return sockfd;

}
void SiTCP::disconnectDevice(){
    if(sockfd>0){
        close(sockfd);
        FD_ZERO(&sockReadSet);
        sockfd = -1;
    }
}
void SiTCP::setFileMaxSize(int n){
    shmp->fileMaxSize = n;
}
void SiTCP::enableBuffer(){
    shmp->writeBuffer = true;
}
void SiTCP::disableBuffer(){
    shmp->writeBuffer = false;
}
void SiTCP::DAQLoop(){
    shmp->dataAcquisitionStatus = status_running;
    // cout<<"DAQ loop start"<<endl;
    int fileSize = 0;
    uint64_t dataSize = 0;
    int nDaq4Count = 1;
    int daqCount = 0;
    // clearDir();
    int length;
    TTimeStamp start_time, now, elapsed;
    start_time.Set();
    int socketBufferSize = 1024*1024;
    char socketBuffer[socketBufferSize];
    while(shmp->dataAcquisitionStatus == status_running){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // sleep(1);
        // if(sockfd<0){
        //     sockfd = connectDevice();
        // }
        // if(sockfd<0){
        //     sleep(1);
        //     std::cout<<"Can Not Connect to SiTCP, Retrying in 1 second..."<<endl;
        //     continue;
        // }
        // length = recv(sockfd, socketBuffer, socketBufferSize, 0);
        // if(length <=0 ){
        //     std::cout<<"sitcp get data length "<<length<<endl;
        //     shmp->rate = length;
        //     continue;
        // }
        // daqCount++;
        // dataSize += length;
        // if(daqCount >= nDaq4Count){
        //     now.Set();
        //     elapsed = now - start_time;
        //     double t = static_cast<double>(elapsed.AsDouble());
        //     if(t == 0){
        //         nDaq4Count++;
        //     }else{
        //         shmp->rate = dataSize /(1024*1024 * t);//MB/s
        //         start_time.Set();
        //         dataSize = 0;
        //         daqCount = 0;
        //     }
        // }
        // if(!file.is_open()){
        //     if(shmp->writeBuffer)createFile();
        // }
        // if(file.is_open()){
        //     file.write(socketBuffer, length);
        //     fileSize += length;
        //     if(fileSize > shmp->fileMaxSize){
        //         closeFile();
        //         fileSize = 0;
        //     }
        // }
    }
    // if(file.is_open()) closeFile();
    // disconnectDevice();
    shmp->rate = 0;
    shmp->dataAcquisitionStatus = status_stopped;
}

void SiTCP::DecodeLoop(LockFreeQueue<LockFreeQueue<TBufferFile*>*> *queue){
    // cout<<"data decoding loop start"<<endl;
    shmp->dataDecodStatus = status_running;
    while(shmp->dataDecodStatus == status_running){
        string name = shmp->dir+to_string(decFileID)+".a";
        if (std::filesystem::exists(name)){
            if(shmp->nQueues>=2){//TODO:
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            // cout<<"decoding file: "<<name<<endl;
            shmp->nTasks++;
            new thread(&SiTCP::DecodeTask, this, decFileID, queue);
            decFileID++;
            if(decFileID == maxFileID) decFileID = 0;
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    while(shmp->nTasks>0){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    shmp->dataDecodStatus = status_stopped;
}

void SiTCP::DecodeTask(int id, LockFreeQueue<LockFreeQueue<TBufferFile*>*> *queue){
    LockFreeQueue<TBufferFile*>* bufferQueue = new LockFreeQueue<TBufferFile*>();
    while(true){
        if(queue->push(bufferQueue, id))break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    shmp->nQueues = queue->getSize();
    PacketDecoder decoder;
    if(id==0)decoder.setFirstEvent(true);
    ifstream file;
    string dir = shmp->dir;
    string filename = dir+to_string(id)+".b";
    rename((dir+to_string(id)+".a").c_str(), filename.c_str());
    file.open(filename.c_str(), std::ios::binary);
    char byte;
    bool endmark;
    while(file.read(&byte,1) && (shmp->dataDecodStatus == status_running)){
        int state = decoder.Fill(byte);
        endmark = false;
        if(state >0) {
            RawEvent* ptr = decoder.getRawEvent();
            TBufferFile* bufferFile = new TBufferFile(TBuffer::kWrite, 1024*1024*20);
            bufferFile->WriteObject(ptr);
            delete ptr;
            bufferQueue->push(bufferFile);
            endmark = true;
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
    int64_t nextID = id+1;
    if(nextID == maxFileID) nextID = 0;
    filename =  dir+to_string(nextID)+".c";
    while(!std::filesystem::exists(filename)){
        if(shmp->dataDecodStatus != status_running || ! shmp->writeBuffer) {
            bufferQueue->stop();
            shmp->nTasks--;
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    file.open(filename.c_str(), std::ios::binary);
    while(file.read(&byte,1) && (shmp->dataDecodStatus == status_running)){
        if(endmark)break;
        int state = decoder.Fill(byte);
        if(state >0) {
            RawEvent* ptr = decoder.getRawEvent();
            TBufferFile* bufferFile = new TBufferFile(TBuffer::kWrite, 1024*1024*20);
            bufferFile->WriteObject(ptr);
            delete ptr;
            bufferQueue->push(bufferFile);
            break ;
        }
        if(state <0) break;
    }
    bufferQueue->stop();
    file.close();
    std::filesystem::path filepath= filename.c_str();
    // std::filesystem::remove(filepath);
    rename(filename.c_str(),(dir+to_string(id+10)+".a").c_str());//TODO: for debug
    shmp->nTasks--;
}
void SiTCP::sendToSiTCP(const char* msg){
    cout<<"send to SiTCP: "<<msg<<endl;
}
void SiTCP::DataSenderLoop(LockFreeQueue<LockFreeQueue<TBufferFile*>*> *queue){
    autoSocket = new AutoSocket(shmp->dataPort);
    LockFreeQueue<TBufferFile*>* bufferQueue;
    TBufferFile * buffer;
    while(shmp->status == status_running){
        if(queue->pop(bufferQueue)){
            while(shmp->status == status_running){
                if(!(bufferQueue->pop(buffer))){
                    if(bufferQueue->isStopped()){
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                while(!autoSocket->send(buffer)){
                    if(shmp->status >= status_stopping)break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                delete buffer;
                if(shmp->status >= status_stopping)break;
            }
            delete bufferQueue;
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        shmp->nQueues = queue->getSize();
    }
    delete autoSocket;
}