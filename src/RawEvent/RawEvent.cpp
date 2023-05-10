#include "RawEvent.h"
ClassImp(RawEvent);

RawEvent::RawEvent(){
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
    channels = new TList();
}
RawEvent& RawEvent::operator=(const RawEvent& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    timestamp = other.timestamp;
    hit_count = other.hit_count;
    for(auto obj: *(other.channels)){
        AddChannel((Channel*)obj);
    }
    return *this;
}
void RawEvent::AddChannel(Channel* ch){
    channels->Add(new Channel(ch));
}
void RawEvent::AddChannel(const Channel& ch){
     channels->Add(new Channel(ch));
}
void RawEvent::reset(){
    for(auto obj: *channels){
        channels->Remove(obj);
        delete obj;
    }
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
}
RawEvent::~RawEvent(){
    for(auto obj: *channels){
        channels->Remove(obj);
        delete obj;
    }
    delete channels;
}