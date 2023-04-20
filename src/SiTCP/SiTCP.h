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

using namespace std;

class SiTCP:public TObject {
public:
    SiTCP();
    virtual ~SiTCP();
    void setDir(const char* dir);
    void connectToDevice(const char* ip, int port);
    void run();
    void stop();
    void sendToDevice(const char* msg);
    void setSocketBufferSize(int n=1024*1024);
    void setFileMaxSize(int n=1024*1024*1024);
    void setFilePrefix(const char* prefix);

private:
    string prefix;
    int fileMaxSize;
    char* socketBuffer=NULL;
    void createFile();
    void closeFile();
    ofstream file;
    string dir;
    int fileID;
    int socketBufferSize;
    void createSocket();
    void updateFileID();

    struct sockaddr_in server_address;
    
    int sock;

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;

    int connected = 1;
    int disconnected = 0;
    atomic_int status;
    atomic_int connectionStatus;

    mutex lock;

    void DAQLoop();
    thread * DAQThread;

    ClassDef(SiTCP,1)
};

#endif
