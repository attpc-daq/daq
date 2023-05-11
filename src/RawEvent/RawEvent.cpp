#include "RawEvent.h"
ClassImp(RawEvent);

RawEvent::RawEvent(){
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
    NChannel = 0;
    channels = new TClonesArray(Channel::Class(),2048);
}
RawEvent& RawEvent::operator=(const RawEvent& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    timestamp = other.timestamp;
    hit_count = other.hit_count;
    // for(auto obj: *(other.channels)){
    //     AddChannel((Channel*)obj);
    // }
    *channels = *(other.channels);
    return *this;
}
void RawEvent::AddChannel(Channel* ch){
    *(Channel*)channels->ConstructedAt(NChannel++) = *ch;
    // TClonesArray &_channels = *channels;
    // Channel *ch = new(_channels[fNChannel++]) Channel(_ch);
}
void RawEvent::AddChannel(const Channel& ch){
    *(Channel*)channels->ConstructedAt(NChannel++) = ch;
    // TClonesArray &_channels = *channels;
    // Channel *ch = new(_channels[fNChannel++]) Channel(_ch);
}
void RawEvent::reset(){
    // for(auto obj: *channels){
    //     channels->Remove(obj);
    //     delete obj;
    // }
    delete channels;
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
    NChannel = 0;
    channels = new TClonesArray(Channel::Class(),2048);
}
RawEvent::~RawEvent(){
    // for(auto obj: *channels){
    //     channels->Remove(obj);
    //     delete obj;
    // }
    delete channels;
}