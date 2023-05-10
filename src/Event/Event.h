#ifndef __Event_h__
#define __Event_h__
#include "TGClient.h"
#include "TObject.h"
#include "Pad.h"
#include <TList.h>

using namespace std;

class Event :public TObject{
public:
  Event();
  Event& operator=(const Event& other);
  virtual ~Event();
  void reset();
  void AddPad(Pad* pad);
  void AddPad(const Pad& pad);
  uint64_t event_id;
  uint64_t timestamp;
  float WValue; //unit: eV
  float Vdrift; //unit: mm/ns
  TList* pads;
private:
  ClassDef(Event,1)
};
#endif
