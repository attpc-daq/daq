#include "Event.h"
#include <iostream>
using namespace std;

ClassImp(Event);

Event::Event(){
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
    mark_count = 0;
    pads.reserve(2048);
}
Event::~Event(){}

Event& Event::operator=(const Event& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    WValue = other.WValue;
    Vdrift = other.Vdrift;
    mark_count = other.mark_count;
    pads = other.pads;
    return *this;
}
void Event::AddPad(Pad* pad){
    pads.push_back(*pad);
}
void Event::AddPad(const Pad& pad){
    pads.push_back(pad);
}
void Event::reset(){
    pads.clear();
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
    mark_count = 0;
}