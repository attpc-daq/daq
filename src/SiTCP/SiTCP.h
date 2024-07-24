#ifndef __SiTCP_h__
#define __SiTCP_h__
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "RawEvent.h"
#include <TMessage.h>
#include <TSocket.h>
#include "TMessageBufferTP.h"
#include "BufferTP.h" //TODO: update 20240724
#include "AutoSocket.h"

using namespace std;

class SiTCP{
public:
    SiTCP(int id);
    virtual ~SiTCP();

    void start();
    void stop();
    void run();

    void startData();
    void stopData();
    void startDecoder();
    void stopDecoder();

    void setDir(const char* dir="./output");
    void clearDir();

    void setDataPort(int port);
    void setFileMaxSize(int n=16*1024*1024);
    int getState();
    int getDataAcquisitionState(){return shmp->dataAcquisitionStatus;}
    int getDecoderState(){return shmp->dataDecodStatus;}
    float getRate();
    int getNTasks();
    void enableBuffer();
    void disableBuffer();
    void setServerAddress(const char* ip, int port);
    void resetSHM();
    void sendToSiTCP(const char* msg);
    string getIP(){return shmp->FEEAddressStr;}
    int getPort(){return shmp->FEEPort;}

private:
    static vector<SiTCP*> instances;
    struct shmseg {
        atomic_int status;
        atomic_int nTasks;
        float rate;
        atomic_bool writeBuffer;
        atomic_int FEEPort;
        unsigned long FEEAddress;
        char FEEAddressStr[24];
        char dir[256];
        atomic_int dataPort;
        atomic_int fileMaxSize;
        atomic_int dataAcquisitionStatus;
        atomic_int dataDecodStatus;
    };
    struct shmseg *shmp;
    pid_t DecoderPID,DAQPID;
    int keyID, shmid;
    pid_t runPID = -1;
    AutoSocket* autoSocket=NULL;

    // string IP;
    // int Port;
    int connectDevice();
    void disconnectDevice();

    int dataPort;

    bool firstFile = true;

    int fileMaxSize = 16*1024*1024;
    // char* socketBuffer=NULL;
    void createFile();
    void closeFile();
    ofstream file;
    // string dir;
    uint64_t daqFileID;
    uint64_t decFileID;
    uint64_t maxFileID = std::numeric_limits<uint64_t>::max();
    
    struct sockaddr_in server_address;
    
    int sockfd = -1;
    fd_set sockReadSet;

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;

    mutex mtx;
    condition_variable cv;

    void DAQLoop();
    void DecodeLoop();
    void DecodeTask(int id);
};
#endif
