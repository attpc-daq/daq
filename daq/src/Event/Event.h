#ifndef __Event_h__
#define __Event_h__
#include "TGClient.h"
#include "TObject.h"
#include "Pad.h"
#include <TClonesArray.h>
// #include <TList.h>
#include <vector>

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
  int mark_count;
  float WValue; //unit: eV
  float Vdrift; //unit: mm/ns
  vector<Pad> pads;
private:
  ClassDef(Event,1)
};
#endif
