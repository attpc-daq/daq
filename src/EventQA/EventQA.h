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
#include "EventMaker.h"
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

private:
  string dir;
  THttpServer * TServ;
  void fill(const RawEvent &revt, const Event &evt);
  TH2D* track_2D = NULL;
  TH3D* track_3D = NULL;
  TH1D* Mesh_Energy_Spectrum = NULL;
  TH1D* Mesh_ADC_Spectrum = NULL;
  TH1D* Pad_ADC = NULL;

  int pad_numQA = 0;

  EventMaker maker;

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
