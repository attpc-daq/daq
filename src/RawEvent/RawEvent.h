#ifndef __RawEvent_h__
#define __RawEvent_h__
#include <TGClient.h>
#include <TObject.h>
#include "Channel.h"
#include <TList.h>
using namespace std;
class RawEvent :public TObject{
public:
  RawEvent();
  RawEvent& operator=(const RawEvent& other);
  virtual ~RawEvent();
  uint64_t event_id;
  uint64_t timestamp;
  uint hit_count;
  void AddChannel(Channel* ch);
  void AddChannel(const Channel& ch);
  void reset();
  TList * channels;
private:
  ClassDef(RawEvent,1)
};
#endif
