#ifndef __PacketDecoder_h__
#define __PacketDecoder_h__
#include "TGClient.h"
#include "TObject.h"
#include "RawEvent.h"
#include "Event.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include <deque>

using namespace std;

class PacketDecoder :public TObject{
public:
  PacketDecoder();

  virtual ~PacketDecoder();

  bool Fill(const char* dataByte);
  RawEvent rawEvent;

  
  uint64_t temp_event_id;
  
  uint64_t temp_timestamp;

private:
  uint64_t _event_id;
  uint64_t _timestamp;
  uint packetType;
  uint packetSize;
  uint packetPose;
  bool firstPacket;
  bool firstEvent;
  bool firstHead;

  uint FEE_ID;
  //UPDATE: 新增3个判断符

  uint findpacket;
  uint findwhat;
  int fillStatusCode;
  int waveformFillStatusCode;
  Channel channel;

  ClassDef(PacketDecoder,1)
};
#endif
