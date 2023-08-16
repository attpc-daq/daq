#include "Event.h"
#include <iostream>
using namespace std;

ClassImp(Event);

Event::Event(){
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
    NPad = 0;
    pads = new TClonesArray(Pad::Class(),2048);//TODO:Pad最大数量是多少？
}
Event::~Event(){
    delete pads;
}

Event& Event::operator=(const Event& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    WValue = other.WValue;
    Vdrift = other.Vdrift;
    *pads = *(other.pads);
    NPad = other.NPad;
    return *this;
}
void Event::AddPad(Pad* pad){
    *(Pad*)pads->ConstructedAt(NPad++) = *pad;
}
void Event::AddPad(const Pad& pad){
    *(Pad*)pads->ConstructedAt(NPad++) = pad;
}
void Event::reset(){
    pads->Clear();
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
    NPad = 0;
}