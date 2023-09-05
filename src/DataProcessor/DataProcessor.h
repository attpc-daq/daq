#ifndef __DataProcessor_h__
#define __DataProcessor_h__
#include "TGClient.h"
#include "TObject.h"
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
#include <THttpServer.h>
#include <filesystem>
#include <TFileCollection.h>
#include "ParameterGenerator.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;
class DataProcessor :public TObject{
public:
  DataProcessor();
  virtual ~DataProcessor();
  void stop();
  void run();
  
  void setDir(const char* dir="./output");
  void setDataPort(int port=8002);
  void setQAPort(int port=8003);
  void setFileEvents(uint64_t n=1000);

  void setRawEventFilePrefix(const char* prefix="rawEvent");
  void setRawEventTreeName(const char* name="rawEvent");
  void setRawEventBranchName(const char* name="rawEvent");

  void setEventFilePrefix(const char* prefix="event");
  void setEventTreeName(const char* name="event");
  void setEventBranchName(const char* name="event");

  void generateParameters(int n=10);

  void setRawEventSave(bool k=false);
  void setEventSave(bool k=false);
  void setQA(bool k=false);

  string getRawEventFileList(int n = -1);
  string getEventFileList(int n = -1);

  uint64_t getNRawEventFiles(){return rawEventFileID;}

  int getState(){return status;}
  // float getRate();
  uint64_t getTotalEvent(){return totalEvent;}
  uint64_t getCurrentTotalEventID(){return currentEventID;}
  int getNMakePar(){return nMakePar;}

  //update: by whk
  void setWValue(float value){WValue = value;}
  void setVdrift(float value){Vdrift = value;}
  void setFPC2(std::vector<std::map<string,int>> fpc2);
  void setGainFile(const char* E_file,const char* M_file){ElectronicFile=E_file;MicromegasFile=M_file;}
  void setElectronicFile(const char* E_file){ElectronicFile=E_file;}
  void setMicromegasFile(const char* M_file){MicromegasFile=M_file;}

private:
  uint64_t totalEvent;
  uint64_t currentEventID;
  vector<RawEvent*> rawEventBuffer;
  int BufferMark[10000];
  //UPDATE: by whk
  float WValue; //unit: eV
  float Vdrift; //unit: mm/ns
  std::vector<std::map<string,int>> FPC2;
  const char* ElectronicFile = nullptr;
  const char* MicromegasFile = nullptr;

  // float rate;
  // uint64_t rateCount;

  EventConverter * converter=NULL;

  TMessageSocket* QASocket=NULL;

  ParameterGenerator parameter;

  TFile * rawEventFile;
  TTree* rawEventTree;
  TFile * eventFile;
  TTree* eventTree;

  RawEvent rawEvent;
  Event event;

  uint64_t rawEventCount=0;
  uint64_t eventCount=0;
  uint64_t rawEventFileID=0;
  uint64_t eventFileID=0;

  int QAPort;
  int dataPort;

  atomic_bool kRawEventSave;
  atomic_bool kEventSave;
  atomic_bool kQA;
  atomic_int nMakePar=0;

  string rawEventBranchName;
  string rawEventTreeName;
  string rawEventFilePrefix;

  string eventBranchName;
  string eventTreeName;
  string eventFilePrefix;

  string dir;
  uint64_t nEvents;

  void loop();
  void saveRawEvent(RawEvent* revt);
  void saveEvent(RawEvent* revt);
  void makePar(RawEvent* revt);
  void QA(RawEvent* revt);
  void updateRawEventFileID();
  void createRawEventFile();
  void closeRawEventFile();
  void updateEventFileID();
  void createEventFile();
  void closeEventFile();

  int status_not_started = 0;
  int status_starting = 1;
  int status_running = 2;
  int status_stopping = 3;
  int status_stopped = 4;
  atomic_int status;
  thread * mThread;

  TTimeStamp start_time, now, elapsed;

  TServerSocket* ss;
  TMonitor serverMonitor, clientMonitor;

  ClassDef(DataProcessor,1)
};
#endif
