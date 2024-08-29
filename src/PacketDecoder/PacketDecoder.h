#ifndef __PacketDecoder_h__
#define __PacketDecoder_h__
#include "RawEvent.h"
#include "Event.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include <deque>

using namespace std;

class PacketDecoder {
public:
  PacketDecoder();
  virtual ~PacketDecoder();
  int Fill(char dataByte, bool debug=false);
  RawEvent* getRawEvent();

private:
  RawEvent *rawEvent;
  uint64_t current_event_id;
  uint64_t timestamp;
  bool firstEvent;
  uint FEE_ID;
  int waveformFillStatusCode;
  enum kPacket{
    notPacket = 0,
    unknownPacket = 1,
    headerPacket = 2,
    bodyPacket = 3,
    endPacket = 4
  };
  kPacket packetType;
  uint packetSize;
  uint packetPose;

  Channel channel;
};
#endif
