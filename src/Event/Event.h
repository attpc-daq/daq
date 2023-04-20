#ifndef __Event_h__
#define __Event_h__
#include "TGClient.h"
#include "TObject.h"
#include "Pad.h"

#include <vector>
using namespace std;

class Event :public TObject{
public:
  Event();
  virtual ~Event();
  Long64_t event_id;
  float WValue; //unit: eV
  float Vdrift; //unit: mm/ns
  vector<Pad> pads;
private:
  ClassDef(Event,1)
};
#endif
