#include "RawEvent.h"
#include <iostream>

ClassImp(RawEvent);

RawEvent::RawEvent(){
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
    vector<Channel>().swap(channels);
}

bool RawEvent::Fill(const Channel& ch){
    if(channels.size()==0){
        event_id = ch.event_id;
        timestamp = ch.timestamp;
    }
    if(event_id != ch.event_id){
        return false;
    }
    channels.push_back(ch);
    return true;
}
bool RawEvent::fill(long _event_id, long _timestamp, long _FEE_id, long _channel_id, float* waveform){
    if(channels.size()==0){
        event_id = _event_id;
        timestamp = _timestamp;
    }
    if(event_id != _event_id){
        return false;
    }

    Channel ch;
    ch.event_id = _event_id;
    ch.timestamp = _timestamp;
    ch.FEE_id = _FEE_id;
    ch.channel_id = _channel_id;
    for(int i = 0;i<1024;i++){
        ch.waveform[i]=waveform[i];
    }
    channels.push_back(ch);

    return true;
}
void RawEvent::reset(){
    vector<Channel>().swap(channels);
    event_id = -1;
    timestamp = 0;
    hit_count = 0;
}

RawEvent::~RawEvent(){}
