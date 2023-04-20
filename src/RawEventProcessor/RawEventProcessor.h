#ifndef __RawEventProcessor_h__
#define __RawEventProcessor_h__
#include "TGClient.h"
#include "TObject.h"
#include "RawEvent.h"
#include "Event.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "Pad.h"
#include "FullEvent.h"
#include "EventMaker.h"
#include <fcntl.h>
#include <filesystem>

#include <semaphore.h>

#include <sys/sem.h>
#include <mutex>
#include <thread>
#include <atomic>
#include "TMessageSocket.h"

using namespace std;

class RawEventProcessor :public TObject{
public:
  RawEventProcessor();
  virtual ~RawEventProcessor();
  void updateSettings(const char* msg);
  void setOutputFilePrefix(const char* prefix);
  void setDir(const char* dir);
  void setRawEventFilePrefix(const char* prefix);
  string getEventFileList(int n = -1);
  void stop();
  void run();
  void setRawTreeName(const char* name);
  void setRawBranchName(const char* name);
  void setEventTreeName(const char* name);
  void setEventBranchName(const char* name);
  // void setSocketPort(int port);

private:
  // int socketPort;

  void mLoop();
  void updateOutputFileID();


  void closeEventFile();
  void closeRawEventFile();

  void createEventFile();
  void openRawEventFile();
  TFile *eventFile;
  TFile *rawEventFile;

  int outputFileID;

  EventMaker maker;

  TString eventTreeName;
  TString eventBranchName;
  TString rawTreeName;
  TString rawBranchName ;

  string outputFilePrefix;
  string dir;
  string rawEventFilePrefix;

  Event event;
  RawEvent *rawEvent;

  TTree *eventTree = NULL;
  TTree *rawTree = NULL;

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;
    atomic_int status;
    mutex lock;
    thread * mThread;
  
    sem_t *  SEMFileID;
  ClassDef(RawEventProcessor,1)
};
#endif
