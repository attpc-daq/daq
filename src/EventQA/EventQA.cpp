#include "EventQA.h"
#include <iostream>
#include "TRandom3.h"
#include "TFile.h"

ClassImp(EventQA);

EventQA::EventQA(){
    Pad_ADC = new TH1D("Pad_ADC","Pad_ADC",4096,0,4095);//ADC码值
    Mesh_Energy_Spectrum = new TH1D("Mesh_Energy_Spectrum","Mesh_Energy_Spectrum",1000,0,10);//unit: MeV
    Mesh_ADC_Spectrum = new TH1D("Mesh_ADC_Spectrum","Mesh_ADC_Spectrum",4096,0,4095);
    const Int_t XNbins=32;
    Double_t XEdge[XNbins+1]={0};
    for(int i=0;i<=XNbins;i++){
        if(i<=5)XEdge[i]=6*i;
        else if(5<i&&i<=9)XEdge[i]=6*5+(i-5)*5;
        else if(9<i&&i<=12)XEdge[i]=6*5+5*4+(i-9)*4;
        else if(12<i&&i<=14)XEdge[i]=6*5+5*4+4*3+(i-12)*3;
        else if(14<i&&i<=18)XEdge[i]=6*5+5*4+4*3+3*2+(i-14)*2;
        else if(18<i&&i<=20)XEdge[i]=6*5+5*4+4*3+3*2+2*4+(i-18)*3;
        else if(20<i&&i<=23)XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+(i-20)*4;
        else if(23<i&&i<=27)XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+4*3+(i-23)*5;
        else XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+4*3+5*4+(i-27)*6;
    }
    
    const Int_t ZNbins=64;
    Double_t ZEdge[ZNbins+1]={0};
    for(int i=0;i<=ZNbins;i++){ZEdge[i]=i*4.5;}

    const Int_t YNbins=180;
    Double_t YEdge[YNbins+1]={0};
    for(int i=0;i<=YNbins;i++){YEdge[i]=i*4.5;}

    track_2D = new TH2D("track_2D","track_2D",ZNbins,ZEdge,XNbins,XEdge);
    track_3D = new TH3D("track_3D","track_3D",ZNbins,ZEdge,YNbins,YEdge,XNbins,XEdge);
    
    setpad_numQA(16,32);
}
EventQA::~EventQA(){

}

string EventQA::get(const char* name){
    if (strcasecmp(name, "track_2D") == 0){
        lock.lock();
        string str = TBufferJSON::ToJSON(track_2D).Data();
        lock.unlock();
        return str;
    }
    if (strcasecmp(name, "track_3D") == 0){
        lock.lock();
         string str = TBufferJSON::ToJSON(track_3D).Data();
        lock.unlock();
        return str;
    }
    if (strcasecmp(name, "Pad_ADC") == 0){
        lock.lock();
        string str =  TBufferJSON::ToJSON(Pad_ADC).Data();
        lock.unlock();
        return str;
    }
    if (strcasecmp(name, "mesh_energy") == 0){
        lock.lock();
        string str =  TBufferJSON::ToJSON(Mesh_Energy_Spectrum).Data();
        lock.unlock();
        return str;
    }
    if (strcasecmp(name, "mesh_adc") == 0){
        lock.lock();
        string str =  TBufferJSON::ToJSON(Mesh_ADC_Spectrum).Data();
        lock.unlock();
        return str;
    }   

    return string("");
}

void EventQA::setMessageHost(int port, const char* host){
    socketPort = port;
    socketHost = host;
}
void EventQA::stop(){
    status = status_stopping;
    mThread->join();
    status = status_stopped;
}
void EventQA::run(){
    status = status_starting;
    mThread = new thread(&EventQA::mLoop, this);
    status = status_running;
}

void EventQA::mLoop(){
    cout<<"Event QA loop start"<<endl;
    status = status_running;
    TMessageSocket *socket = new TMessageSocket(socketPort, socketHost.c_str());
    while(status == status_running){
        RawEvent* revt =(RawEvent*) socket->get(RawEvent::Class());
        if(revt == NULL)continue;
        Event *evt = maker.convert(*revt);
        //---------------------------------
        // Do QA with the event here
        //---------------------------------
        lock.lock();
        fill(*revt, *evt);
        lock.unlock();
    }
    delete socket;
    cout<<"Event QA Loop stop"<<endl;
}
void EventQA::updateSettings(const char* msg){
  maker.updateSettings(msg);
}

void EventQA::fill(const RawEvent &revt, const Event &evt){
    // resetwaveform();
    float mesh_energy = 0;
    float mesh_charge = 0;
    track_2D->Reset();
    track_3D->Reset();
    // int i=0;
    for(auto iter : evt.pads){
        int padrow = iter.padRow;
        int padcol = iter.padColumn;
        int adc = iter.adc;
        int height =int(iter.DriftTime*(evt.Vdrift));
        // cout<<iter.DriftTime<<"  "<<evt.event.Vdrift<<endl;
        int charge = iter.ChargeDeposited;
        mesh_energy += iter.Energy;
        mesh_charge += charge;
        // cout<<"iter.Energy: "<<iter.Energy<<endl;

        if(iter.padNumber==pad_numQA)
            Pad_ADC->Fill(adc);
        // track_2D->SetBinContent(padcol+1,padrow+1,adc);
        // track_3D->SetBinContent(padcol+1,padrow+1,180-height,adc);
        track_2D->SetBinContent(padcol+1,padrow+1,charge);
        track_3D->SetBinContent(padcol+1,padrow+1,180-height,charge);
        
        // gr[i] = new TGraph();
        // gr[i]->SetName(Form( "event-%i: raw-%i col-%i",evt.event.event_id,padrow,padcol));
        // gr[i]->SetTitle(Form("event-%i: raw-%i col-%i",evt.event.event_id,padrow,padcol));
        // for(int j=0;j<1024;j++){
        //     int waveform = evt.raw_event.channels[i].waveform[j];
        //     gr[i]->SetPoint(j,j,waveform);
        // }
        // mg->Add(gr[i]);
        // i++;
    }
    // cout<<"=================>>>mesh_energy: "<<mesh_energy<<endl;
    Mesh_ADC_Spectrum->Fill(mesh_charge*1E+15/6.24150975E+18*0.75/10*4096/4000);//mesh_charge*1E+15/6.24150975E+18 unit:fc  假设ADC量程为4V=4000mV，精度为12bit  主放增益为10，前放增益为0.75mv/fc
    Mesh_Energy_Spectrum->Fill(mesh_energy);//unit: MeV
}
void EventQA::setpad_numQA(long num){
    Pad_ADC->Reset();
    pad_numQA = num;
}

void EventQA::setpad_numQA(long row, long column){
    //row: 0~31 column: 0~63
    Pad_ADC->Reset();
    pad_numQA = row*64+column;
}
