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
#include <condition_variable>
#include <THttpServer.h>
#include <filesystem>
#include <TFileCollection.h>
#include "ParameterGenerator.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "AutoSocket.h"
#include<deque>

using namespace std;
class DataProcessor :public TObject{
public:
  DataProcessor();
  virtual ~DataProcessor();
  void stop();
  void run();
  
  void setDir(const char* dir="./output");
  void setDataPort(int port1=8010,const char* host1="localhost",int port2=8011, const char* host2="localhost");
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

  uint64_t getOutputFileID(){return outputFileID;}

  int getState(){return status;}

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

  int dataPort1=8010;
  string dataHost1="localhost";
  int dataPort2=8011;
  string dataHost2="localhost";

  deque<RawEvent**> bufferDQ01;
  deque<RawEvent**> bufferDQ02;
  deque<RawEvent**> bufferDQ;

  void msgReceiver(int port, const char* host, deque<RawEvent**> * bufferDQ);
  void assemble();
  void rawEventProcess(RawEvent** ptrArray, int fileID);
  void bufferProcess();
  uint64_t totalEvent;
  uint64_t currentEventID;

  //UPDATE: by whk
  float WValue; //unit: eV
  float Vdrift; //unit: mm/ns
  std::vector<std::map<string,int>> FPC2;
  const char* ElectronicFile = nullptr;
  const char* MicromegasFile = nullptr;

  EventConverter * converter=NULL;

  TMessageSocket* QASocket=NULL;

  ParameterGenerator parameter;

  uint64_t rawEventFileID=0;
  uint64_t eventFileID=0;
  uint64_t outputFileID=0;

  int QAPort;

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

  void makePar(RawEvent* revt);
  void updateRawEventFileID();
  void updateEventFileID();

  int status_not_started = 0;
  int status_starting = 1;
  int status_running = 2;
  int status_stopping = 3;
  int status_stopped = 4;
  atomic_int status;
  
  thread* mMsgThread1;
  thread* mMsgThread2;
  thread* mAssembleThread;
  thread* mBufferProcess;

  mutex mtx;
  condition_variable cv;
  ClassDef(DataProcessor,1)
};
#endif
