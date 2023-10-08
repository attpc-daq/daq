#include "Channel.h"
ClassImp(Channel);
Channel::Channel(){}
Channel::~Channel(){}
Channel::Channel(const Channel& other):
event_id(other.event_id),
timestamp(other.timestamp),
FEE_id(other.FEE_id),
channel_id(other.channel_id)
{
    memcpy(waveform,other.waveform,1024*sizeof(int));
    // for(int i=0;i<1024;i++){
    //     waveform[i] = other.waveform[i];
    // }
}
Channel::Channel(Channel* other):
event_id(other->event_id),
timestamp(other->timestamp),
FEE_id(other->FEE_id),
channel_id(other->channel_id)
{
    memcpy(waveform,other->waveform,1024*sizeof(int)); 
    // for(int i=0;i<1024;i++){
    //     waveform[i] = other->waveform[i];
    // }
}