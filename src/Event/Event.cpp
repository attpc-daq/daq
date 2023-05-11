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
    // for(auto obj: *pads){
    //     pads->Remove(obj);
    //     delete obj;
    // }
    delete pads;
}

Event& Event::operator=(const Event& other){
    if(this == &other)return *this;
    reset();
    event_id = other.event_id;
    WValue = other.WValue;
    Vdrift = other.Vdrift;
    // for(auto obj: *pads){
    //     AddPad((Pad*)obj);
    // }
    *pads = *(other.pads);
    return *this;
}
void Event::AddPad(Pad* pad){
    // *(Pad*)pads->ConstructedAt(NPad++) = *pad;
    TClonesArray &_pads = *pads;
    new(_pads[NPad++]) Pad(pad);
    // pads->Add(new Pad(ch));
}
void Event::AddPad(const Pad& pad){
    // *(Pad*)pads->ConstructedAt(NPad++) = pad;
    TClonesArray &_pads = *pads;
    new(_pads[NPad++]) Pad(pad);
    // pads->Add(new Pad(ch));
}
void Event::reset(){
    // for(auto obj: *pads){
    //     pads->Remove(obj);
    //     delete obj;
    // }
    delete pads;
    event_id = 0;
    timestamp = 0;
    WValue = 30.0;
    Vdrift = 0;
    NPad = 0;
    pads = new TClonesArray(Pad::Class(),2048);//TODO:Pad最大数量是多少？
}