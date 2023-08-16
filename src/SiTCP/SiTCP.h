#ifndef __SiTCP_h__
#define __SiTCP_h__
#include "TGClient.h"
#include "TObject.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>

#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>
#include "RawEvent.h"
#include <TMessage.h>
#include <TSocket.h>
#include <deque>

using namespace std;

class SiTCP:public TObject {
public:
    SiTCP();
    virtual ~SiTCP();
    void setDir(const char* dir="./output");
    void clearDir();

    void run();
    void stop();
    // bool sendToDevice(const char* msg);
    void setDataProcessHost(int port=8002, const char* host="localhost");
    void setSocketBufferSize(int n=1024*1024);
    void setFileMaxSize(int n=16*1024*1024);
    int getState(){return status;}
    int getConnectionState(){return connectionStatus;}
    float getRate(){return rate;}
    int getNTasks(){return nTasks;}
    bool getDecoderState(){return writeBuffer;}
    void enableDecoder();
    void disableDecoder();
    void setupServerAddress(const char* ip, int port);

private:
    void connectDevice();
    void disconnectDevice();

    int dataPort;
    string dataHost;
    
    std::deque<TSocket*> sockDeque;

    bool firstFile;

    float rate;
    int fileMaxSize;
    char* socketBuffer=NULL;
    void createFile();
    void closeFile();
    ofstream file;
    string dir;
    uint64_t daqFileID;
    uint64_t decFileID;
    uint64_t maxFileID;
    int socketBufferSize;
    void createSocket();
    
    struct sockaddr_in server_address;
    
    int sock;
    int max_fd;
    int activity;
    fd_set sockReadSet,sockWriteSet;

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;

    int connected = 1;
    int disconnected = 0;
    atomic_int status;
    atomic_int connectionStatus;
    atomic_int nTasks;

    atomic_bool writeBuffer;

    mutex lock;

    void DAQLoop();
    void DecodeLoop();
    void DecodeTask(int id, TSocket* rootsock);
    thread * DAQThread;
    thread * DecodeThread;

    ClassDef(SiTCP,1)
};

#endif
