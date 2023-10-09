#ifndef __DataProcessor_h__
#define __DataProcessor_h__
#include "TMultiGraph.h"
#include "TGraph.h"
#include "Event.h"
#include <TH1D.h>
#include <TFile.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TBufferJSON.h>
#include "RawEvent.h"
#include "TMessageSocket.h"
#include "EventConverter.h"
#include <mutex>
#include <condition_variable>
#include <THttpServer.h>
#include <filesystem>
#include <TFileCollection.h>
#include "ParameterGenerator.h"
#include <TTree.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "AutoSocket.h"
#include <deque>

using namespace std;
class DataProcessor {
public:
  DataProcessor(int id=3);
  virtual ~DataProcessor();

  void start();
  void stop();
  void run();
  int getState(){return shmp->status;}
  
  void setDir(const char* dir="./output");
  string getDir(){return shmp->dir;}
  void setDataPort(int port1=8010,const char* host1="localhost",int port2=8011, const char* host2="localhost");
  void setFileEvents(int n=1000);
  void generateParameters(int n=10);
  void setRawEventSave(bool k=false);
  void setEventSave(bool k=false);
  int getOutputFileID();
  int getTotalEvent();
  int getCurrentEventID();
  int getNMakePar();
  //update: by whk
  void setWValue(float value){shmp->WValue = value;}
  void setVdrift(float value){shmp->Vdrift = value;}
  void setFPC2(std::vector<std::map<string,int>> fpc2);
  void setGainFile(const char* E_file,const char* M_file);
  void setElectronicFile(const char* E_file);
  void setMicromegasFile(const char* M_file);
  void resetSHM();

private:
  struct shmseg {
    atomic_bool kRawEventSave;
    atomic_bool kEventSave;
    atomic_int nMakePar;
    atomic_int status;
    atomic_int totalEvent;
    atomic_int currentEventID;
    atomic_int outputFileID;
    char dir[256];
    atomic_int dataPort1;
    char dataHost1[20];
    atomic_int dataPort2;
    char dataHost2[20];
    atomic_int nEvents;
    //TODO: 运行中需要更新的参数要写在共享内存中
    float WValue;
    float Vdrift;
    char ElectronicFile[256];
    char MicromegasFile[256];
    char FPC2[1024];
  };
  int shmid;
  struct shmseg *shmp;
  int keyID;
  pid_t runPID=-1;
  deque<RawEvent**> rawSubEventBufferDQ;

  void msgReceiver();
  void rawEventProcess(RawEvent** ptrArray, int fileID);
  void bufferProcess();

  //UPDATE: by whk
  // float WValue; //unit: eV
  // float Vdrift; //unit: mm/ns
  // std::vector<std::map<string,int>> FPC2;
  // const char* ElectronicFile = nullptr;
  // const char* MicromegasFile = nullptr;

  EventConverter * converter=NULL;

  ParameterGenerator parameter;

  uint64_t rawEventFileID=0;
  uint64_t eventFileID=0;

  string rawEventBranchName;
  string rawEventTreeName;
  string rawEventFilePrefix;

  string eventBranchName;
  string eventTreeName;
  string eventFilePrefix;

  void makePar(RawEvent* revt);
  void updateRawEventFileID();
  void updateEventFileID();

  int status_not_started = 0;
  int status_starting = 1;
  int status_running = 2;
  int status_stopping = 3;
  int status_stopped = 4;
  
  thread* mMsgThread;
  thread* mAssembleThread;
  thread* mBufferProcess;

  mutex mtx;
  condition_variable cv;
};
#endif
