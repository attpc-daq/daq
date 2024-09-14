#ifndef __EventQA_h__
#define __EventQA_h__

#include "TMultiGraph.h"
#include "TGraph.h"
#include "Event.h"
#include <TH1D.h>
#include <TFile.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TBufferJSON.h>
#include "RawEvent.h"
#include "EventConverter.h"
#include <mutex>
#include <THttpServer.h>
#include <filesystem>
#include <TFileCollection.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

class EventQA {
public:
  EventQA(bool debug = false);
  virtual ~EventQA();
  void start();
  void stop();
  void run();
  void setTHttpServerPort(int port=8008);
  string get(const char* name);
  void updateSettings(const char* msg);
  void setpad_numQA(long num);//num: 0~2047 num=row*64+column
  void setpad_numQA(long row, long column);//row: 0~31 column: 0~63
  string getList();
  void setDir(const char* dir);
  int getState(){return status;}
  uint64_t getTotalEvent(){return totalEvent;}
  uint64_t getCurrentEventID(){return currentEventID;}

  void clearPlots();
  void doFirstFile();
  void doFirstEvent();
  void doPreviousFile();
  void doPreviousEvent();
  void doFile(int fileID);
  void doEvent(int entryID);
  void doNextFile();
  void doNextEvent();
  void doLastFile();
  void doLastEvent();
  void setAutoFile();
  int getCurrentFileID(){return currentRawEventFileID;}
  int getCurrentEventEntryID(){return currentEventEntryID;}
  bool getAutoFileMode(){return autoFileMode;}

private:
  bool kDebug;
  RawEvent *rawEvent;
  Event *event;
  TFile* file=NULL;
  TTree* tree;
  int getLastID();
  int getFirstID();

  bool autoFileMode = false;
  thread * mQAThread = NULL;
  thread * mTServThread = NULL;
  atomic_int status;
  atomic_int totalEvent;
  atomic_int currentEventID;
  atomic_int currentEventEntryID;
  atomic_int currentRawEventFileID;
  int THttpServerPort;
  string dir;
  string rawEventFilePrefix;
  string eventFilePrefix;

  THttpServer* TServ = NULL;

  void fill(const RawEvent &revt, const Event &evt);
  void fill();
  TH2D* track_2D_ZX = NULL;
  TH2D* track_2D_ZY = NULL;
  TH2D* track_2D_XY = NULL;
  TH3D* track_3D = NULL;
  TH1D* Mesh_Energy_Spectrum = NULL;
  TH1D* Mesh_ADC_Spectrum = NULL;
  TH1D* Mesh_charge_Spectrum = NULL;
  TH1D* Pad_ADC = NULL;
  TH1I* event_time = NULL;
  TMultiGraph* mg = NULL;
  TGraph* gr[2048];
  const int colorindex[12] = {
      kBlack, kRed-4, kGreen, kBlue-4, kYellow, kMagenta-3,
      kCyan, kOrange, kTeal-6, kAzure, kViolet, kPink+9
  };

  int pad_numQA = 0;
  EventConverter converter;

  void mQALoop();
  string getRawEventFileName();
  string getEventFileName();

  int status_not_started = 0;
  int status_starting = 1;
  int status_running = 2;
  int status_stopping = 3;
  int status_stopped = 4;
  mutex lock;
};
#endif
