#include "Event.h"
#include <iostream>
using namespace std;

ClassImp(Event);

Event::Event(){
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
    pads = new TList();
}
Event::~Event(){
    for(auto obj: *pads){
        pads->Remove(obj);
        delete obj;
    }
    delete pads;
}

Event& Event::operator=(const Event& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    WValue = other.WValue;
    Vdrift = other.Vdrift;
    for(auto obj: *pads){
        AddPad((Pad*)obj);
    }
    return *this;
}
void Event::AddPad(Pad* ch){
    pads->Add(new Pad(ch));
}
void Event::AddPad(const Pad& ch){
    pads->Add(new Pad(ch));
}
void Event::reset(){
    for(auto obj: *pads){
        pads->Remove(obj);
        delete obj;
    }
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
}