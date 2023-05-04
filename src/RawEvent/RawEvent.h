#ifndef __RawEvent_h__
#define __RawEvent_h__
#include "TGClient.h"
#include "TObject.h"
#include "Channel.h"
#include <vector>

using namespace std;
class RawEvent :public TObject{
public:
  RawEvent();
  virtual ~RawEvent();
  //UPDATE： timestamp[47-0 bit] 需要用无整形uint64_t表示
  uint64_t event_id;
  uint64_t timestamp;
  int hit_count;
  bool Fill(const Channel& ch);
  bool fill(long event_id, long timestamp, long FEE_id, long channel_id, float* waveform);
  void reset();
  vector<Channel> channels;
private:
  ClassDef(RawEvent,1)
};
#endif
