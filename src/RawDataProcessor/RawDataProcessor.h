#ifndef __RawDataProcessor_h__
#define __RawDataProcessor_h__
#include "TGClient.h"
#include "TObject.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <atomic>
#include "PacketDecoder.h"
#include "RawEvent.h"

#include <mutex>

#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>

#include "TMessageSocket.h"

using namespace std;

class RawDataProcessor:public TObject {
public:
    RawDataProcessor();
    virtual ~RawDataProcessor();
    void run(bool broadcast= true);
    void stop();
    int state(){return status;}

    void setOutputFileEvents(int n);
    void setOutputDir(const char* dir);
    void setOutputFilePrefix(const char* prefix);

    void setRawEventTreeName(const char* name);
    void setRawEventBranchName(const char* name);

    void setWritingTempFileName(const char* name);

    void setRawDataDir(const char* dir);
    void setRawDataFilePrefix(const char* prefix);
    void setReadingTempFileName(const char* name);
    string getRawDataFileList(int n = -1);
    string getRawEventFileList(int n = -1);
    
    void setSocketPort(int port);
    void clearDir();

private:
    int socketPort;

    string rawEventTreeName;
    string rawEventBranchName;

    PacketDecoder decoder;

    RawEvent rawEvent;

    TFile * rawEventFile;
    TTree * rawTree;

    string writingTempFileName;

    string readingTempFileName;
    string rawDataFilePrefix;
    string rawEventFilePrefix;
    int nEvents;
    uint64_t rawDataFileID; 
    uint64_t outputFileID;
    ifstream rawDataFile;
    string rawDataDir;
    string outputDir;

    bool openRawDataFile();
    void closeRawDataFile();
    void updateRawDataFileID();
    
    void createOutputFile();
    void closeOutputFile();
    void updateOutputFileID();

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;

    atomic_int status;

    mutex lock;

    void mLoop(bool broadcast);
    thread * mThread;

    ClassDef(RawDataProcessor,1)
};

#endif
