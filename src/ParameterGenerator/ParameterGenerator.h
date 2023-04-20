#ifndef __ParameterGenerator_h__
#define __ParameterGenerator_h__
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

#include <fcntl.h>

using namespace std;

class ParameterGenerator:public TObject {
public:
    ParameterGenerator();
    virtual ~ParameterGenerator();
    void run(int nEvent);
    void stop();

    void setOutputDir(const char* dir);
    void setOutputFileName(const char* thresholdFileName, const char* eventParameterFileName);

    void setRawDataDir(const char* dir);
    void setRawDataFilePrefix(const char* prefix);
    void setReadingTempFileName(const char* name);
    bool isFinish();

private:
    string generate_settings(int WValue, int Vdrift);
    int sum_threshold[32][64];
    int count[32][64];

    void fill(RawEvent* revt);
    void make_threshold();
    PacketDecoder decoder;

    RawEvent rawEvent;

    string readingTempFileName;
    string rawDataFilePrefix;

    int thresholdFileID;
    int eventParameterFileID;
    string thresholdFileName;
    string eventParameterFileName;

    int rawDataFileID; 

    ifstream rawDataFile;
    ofstream thresholdFile;
    ofstream eventParameterFile;
    string rawDataDir;
    string outputDir;

    bool openRawDataFile();
    void closeRawDataFile();
    void updateRawDataFileID();
    
    // void createOutputFile();
    // void closeOutputFile();

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;

    atomic_int status;

    mutex lock;

    void mLoop(int nEvent);
    thread * mThread;

    ClassDef(ParameterGenerator,1)
};

#endif
