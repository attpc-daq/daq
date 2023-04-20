#ifndef CHANNEL_H
#define CHANNEL_H
#include "TGClient.h"
#include "TObject.h"
class Channel:public TObject{
public:
    Channel();
    ~Channel();
    Long64_t event_id;
    Long64_t timestamp;
    int FEE_id;
    int channel_id;
    UInt_t waveform[1024];
    ClassDef(Channel,1)
};
#endif // CHANNEL_H
