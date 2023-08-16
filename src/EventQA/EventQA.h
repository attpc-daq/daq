#ifndef __EventQA_h__
#define __EventQA_h__
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

using namespace std;
class EventQA :public TObject{
public:
  EventQA(int port=8008);
  virtual ~EventQA();
  void setMessageHost(int port, const char* host="localhost");
  string get(const char* name);
  void stop();
  void run();
  void updateSettings(const char* msg);

  void setpad_numQA(long num);//num: 0~2047 num=row*64+column
  void setpad_numQA(long row, long column);//row: 0~31 column: 0~63

  int state(){return status;}
  string getList();

  void setDir(const char* dir);
  float getRate();

private:

  RawEvent rawEvent;
  Event event;
  TTimeStamp start_time, now, elapsed;
  float rate=0;

  string dir;
  THttpServer * TServ;
  void fill(const RawEvent &revt, const Event &evt);
  TH2D* track_2D = NULL;
  TH3D* track_3D = NULL;
  TH1D* Mesh_Energy_Spectrum = NULL;
  TH1D* Mesh_ADC_Spectrum = NULL;
  TH1D* Pad_ADC = NULL;
  TH1D* Laser_Dir = NULL;
  TMultiGraph* mg = NULL;
  const int colorindex[12] = {
      kBlack, kRed-4, kGreen, kBlue-4, kYellow, kMagenta-3,
      kCyan, kOrange, kTeal-6, kAzure, kViolet, kPink+9
  };

  int pad_numQA = 0;

  EventConverter converter;

  int THttpServerPort;

  int socketPort;
  string socketHost;
  void mQALoop();
  void mTServLoop();

  int status_not_started = 0;
  int status_starting = 1;
  int status_running = 2;
  int status_stopping = 3;
  int status_stopped = 4;
  atomic_int status;
  thread * mQAThread;
  thread * mTServThread;

  mutex lock;
  ClassDef(EventQA,1)
};
#endif
