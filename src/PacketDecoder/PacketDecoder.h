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

private:
  int packetSize;
  int FEE_ID;
  int fillStatusCode;
  int waveformFillStatusCode;
  Channel channel;

  ClassDef(PacketDecoder,1)
};
#endif
