#include "RawEvent.h"
#include <iostream>
using namespace std;

ClassImp(RawEvent);

RawEvent::RawEvent(){
    event_id  = 0;
    timestamp = 0;
    hit_count = 0;
    mark_count = 0;
    channels.reserve(2048);
}
RawEvent& RawEvent::operator=(const RawEvent& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    timestamp = other.timestamp;
    hit_count = other.hit_count;
    mark_count = other.mark_count;
    channels = other.channels;
    return *this;
}
bool RawEvent::Add(RawEvent* revt){
    if(event_id != revt->event_id) return false;
    hit_count += revt->hit_count;
    if(mark_count*revt->mark_count<0){
        mark_count = mark_count * revt->mark_count;
    }else{
        mark_count += revt->mark_count;
    }
    channels.insert(channels.end(),revt->channels.begin(),revt->channels.end());
    return true;
}
void RawEvent::AddChannel(Channel* ch){
    channels.push_back(*ch);
}
void RawEvent::AddChannel(const Channel& ch){
    channels.push_back(ch);
}
void RawEvent::reset(){
    channels.clear();
    event_id  = 0;
    timestamp = 0;
    hit_count = 0;
    mark_count = 0;
}
RawEvent::~RawEvent(){}