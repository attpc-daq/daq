#include "RawEvent.h"
#include <iostream>
using namespace std;

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
    *channels = *(other.channels);
    NChannel = other.NChannel;
    return *this;
}
bool RawEvent::Add(RawEvent* revt){

    if(event_id != revt->event_id) return false;
    // std::cout<<" event_id "<<revt->event_id<<" time "<<revt->timestamp<<" time "<<timestamp<<endl;
    hit_count += revt->hit_count;

    for(int i=0;i<revt->NChannel;i++){
        Channel* ch = (Channel*) revt->channels->At(i);
        AddChannel(ch);
    }

    return true;
}
void RawEvent::AddChannel(Channel* ch){
    *(Channel*)channels->ConstructedAt(NChannel++) = *ch;
}
void RawEvent::AddChannel(const Channel& ch){
    *(Channel*)channels->ConstructedAt(NChannel++) = ch;
}
void RawEvent::reset(){
    channels->Clear();
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
    NChannel = 0;
}
RawEvent::~RawEvent(){
    delete channels;
}