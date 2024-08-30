#ifndef __OnlineQA_h__
#define __OnlineQA_h__

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

class OnlineQA {
public:
  OnlineQA(int id = 4);
  virtual ~OnlineQA();
  void start();
  void stop();
  void run();
  void setTHttpServerPort(int port=8008);
  void setDataPort(int port=8012,const char* host="0.0.0.0");

  void setJsonFilePath(const char* path);
  int getState(){return shmp->status;}
  uint64_t getTotalEvent(){return shmp->totalEvent;}
  uint64_t getCurrentEventID(){return shmp->currentEventID;}

  void clearPlots();
  void resetSHM();
  void setpad_numQA(long num);//num: 0~2047 num=row*64+column
  void setpad_numQA(long row, long column);//row: 0~31 column: 0~63
  void setDebug(bool debug);
  
private:
struct shmseg {
    bool kDebug;
    atomic_int clearPlots;
    atomic_int status;
    atomic_int totalEvent;
    atomic_int currentEventID;
    atomic_int THttpServerPort;
    atomic_int dataPort;
    char dataHost[20];
    char jsonFilePath[256];
  };
  int shmid;
  struct shmseg *shmp;
  int keyID;
  pid_t runPID=-1;
  void TServLoop();
  void dataReceiver();
  void fill(RawEvent *revt = NULL, Event* evt = NULL);

  TH1I* event_id = NULL;

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
  int status_not_started = 0;
  int status_starting = 1;
  int status_running = 2;
  int status_stopping = 3;
  int status_stopped = 4;
  mutex lock;
};
#endif
