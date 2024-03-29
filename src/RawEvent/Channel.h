#ifndef CHANNEL_H
#define CHANNEL_H
#include "TGClient.h"
#include "TObject.h"
class Channel:public TObject{
public:
    Channel();
    Channel(const Channel& other);
    Channel(Channel* other);
    ~Channel();
    uint64_t event_id;
    uint64_t timestamp;
    int FEE_id;
    int channel_id;
    int waveform[1024];
    ClassDef(Channel,1)
};
#endif // CHANNEL_H
