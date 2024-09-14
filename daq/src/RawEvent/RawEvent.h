#ifndef __RawEvent_h__
#define __RawEvent_h__
#include <TGClient.h>
#include <TObject.h>
#include "Channel.h"
#include <TClonesArray.h>
#include <TObjArray.h>
#include <vector>

using namespace std;
class RawEvent :public TObject{
public:
  RawEvent();
  RawEvent& operator=(const RawEvent& other);
  virtual ~RawEvent();
  bool Add(RawEvent* revt);
  uint64_t event_id;
  uint64_t timestamp;
  uint hit_count;
  int mark_count;//此变量记录原始数据中的head-end数量。 =0正常事件，>0 可能有channel丢失并计入到下一个事件，<0 可能有前一个事件的channel混入
  void AddChannel(Channel* ch);
  void AddChannel(const Channel& ch);
  void reset();
  vector<Channel> channels;
private:
  ClassDef(RawEvent,1)
};
#endif
