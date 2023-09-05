#ifndef __EventConverter_h__
#define __EventConverter_h__
#include "TGClient.h"
#include "TObject.h"
#include "RawEvent.h"
#include "Event.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "Pad.h"

using namespace std;

class EventConverter :public TObject{
public:
  EventConverter();
  EventConverter(const char* filename);
  virtual ~EventConverter();
  void updateSettings(const char* jsonString);
  double CFD(UInt_t* waveform, Int_t baseline, uint64_t timestampe, double attenuate_factor=0.5);

  Event* convert(const RawEvent &evt);
  Event *event;

private:
  RawEvent rawEvent;

  //电子学通道映射转换函数
  void PadMap();

  float WValue; //unit: eV  气体平均电离能
  float Vdrift; //unit: mm/ns  电子漂移速度
  int FPC2[32]={0};
  float Electronic_Gain[32][64]={0.};
  float Electronic_time_offset[32][64]={0.};
  float Micromegas_Gain[32][64]={0.};

  //电子学通道映射转换需要的变量
  int XPos[2048];
  int ZPos[2048];

  ClassDef(EventConverter,1)
};
#endif
