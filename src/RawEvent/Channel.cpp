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
    memcpy(waveform,other.waveform,1024*sizeof(UInt_t)); 
}
Channel::Channel(Channel* other):
event_id(other->event_id),
timestamp(other->timestamp),
FEE_id(other->FEE_id),
channel_id(other->channel_id)
{
    memcpy(waveform,other->waveform,1024*sizeof(UInt_t)); 
}