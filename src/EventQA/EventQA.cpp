#include "EventQA.h"
#include <iostream>
#include "TRandom3.h"
#include "TFile.h"
#include "TList.h"
#include "TIterator.h"
#include "TColor.h"
#include "TVector3.h"
#include "AutoSocket.h"
#include "TTimer.h"
#include <TROOT.h>

EventQA::EventQA(){
    ROOT::EnableThreadSafety();
    status = 0;
    totalEvent = 0;
    currentEventID = 0;
    currentRawEventFileID = 0;
    currentEventFileID = 0;
    rawEventFilePrefix = "rawEvent";
    eventFilePrefix = "event";
    Pad_ADC = new TH1D("Pad_ADC","Pad_ADC",4096,0,4095);//ADC码值
    Pad_ADC->GetXaxis()->SetTitle("channels");
    Pad_ADC->GetYaxis()->SetTitle("counts");
    Mesh_Energy_Spectrum = new TH1D("Mesh_Energy_Spectrum","Mesh_Energy_Spectrum",1000,0,10);//unit: MeV
    Mesh_Energy_Spectrum->GetXaxis()->SetTitle("energy(MeV)");
    Mesh_Energy_Spectrum->GetYaxis()->SetTitle("counts");
    Mesh_ADC_Spectrum = new TH1D("Mesh_ADC_Spectrum","Mesh_ADC_Spectrum",4096,0,4095);
    Mesh_ADC_Spectrum->GetXaxis()->SetTitle("channels");
    Mesh_ADC_Spectrum->GetYaxis()->SetTitle("counts");
    const Int_t XNbins=32;
    Double_t XEdge[XNbins+1]={0};
    for(int i=0;i<=XNbins;i++){
        if(i<=5)XEdge[i]=6*i-72;
        else if(5<i&&i<=9)XEdge[i]=6*5+(i-5)*5-72;
        else if(9<i&&i<=12)XEdge[i]=6*5+5*4+(i-9)*4-72;
        else if(12<i&&i<=14)XEdge[i]=6*5+5*4+4*3+(i-12)*3-72;
        else if(14<i&&i<=18)XEdge[i]=6*5+5*4+4*3+3*2+(i-14)*2-72;
        else if(18<i&&i<=20)XEdge[i]=6*5+5*4+4*3+3*2+2*4+(i-18)*3-72;
        else if(20<i&&i<=23)XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+(i-20)*4-72;
        else if(23<i&&i<=27)XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+4*3+(i-23)*5-72;
        else XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+4*3+5*4+(i-27)*6-72;
    }
    
    const Int_t ZNbins=64;
    Double_t ZEdge[ZNbins+1]={0};
    for(int i=0;i<=ZNbins;i++){ZEdge[i]=i*4.5-144.;}

    const Int_t YNbins=180;
    Double_t YEdge[YNbins+1]={0};
    for(int i=0;i<=YNbins;i++){YEdge[i]=i*1.0-90;}

    track_2D_ZX = new TH2D("track_2D_ZX","track_2D_ZX",ZNbins,ZEdge,XNbins,XEdge);
    track_2D_ZX->GetXaxis()->SetTitle("Z [mm]");
    track_2D_ZX->GetYaxis()->SetTitle("X [mm]");
    track_2D_ZY = new TH2D("track_2D_ZY","track_2D_ZY",ZNbins,ZEdge,YNbins,YEdge);
    track_2D_ZY->GetXaxis()->SetTitle("Z [mm]");
    track_2D_ZY->GetYaxis()->SetTitle("Y [mm]");
    track_2D_XY = new TH2D("track_2D_XY","track_2D_XY",XNbins,XEdge,YNbins,YEdge);
    track_2D_XY->GetXaxis()->SetTitle("X [mm]");
    track_2D_XY->GetYaxis()->SetTitle("Y [mm]");
    track_3D = new TH3D("track_3D","track_3D",ZNbins,ZEdge,XNbins,XEdge,YNbins,YEdge);
    track_3D->GetXaxis()->SetTitle("Z [mm]");
    track_3D->GetYaxis()->SetTitle("X [mm]");
    track_3D->GetZaxis()->SetTitle("Y [mm]");

    mg = new TMultiGraph("waveform","waveform");
    mg->GetXaxis()->SetTitle("Time (ns)");
    mg->GetYaxis()->SetTitle("ADC");
    for(int i=0;i<2048;i++){
        gr[i] = NULL;
    }
    setpad_numQA(16,32);
}
EventQA::~EventQA(){
    delete Pad_ADC;
    delete Mesh_Energy_Spectrum;
    delete Mesh_ADC_Spectrum;
    delete track_2D_XY;
    delete track_2D_ZX;
    delete track_2D_ZY;
    delete track_3D;
    delete mg;
}
void EventQA::start(){
    status = status_starting;
    TServ = new THttpServer(Form("http:%d",THttpServerPort));
    TServ->SetReadOnly(kFALSE);
    TServ->Register("",mg);
    TServ->CreateServerThread();

    mQAThread = new thread(&EventQA::mQALoop, this);
    while(status != status_running){
        sleep(1);
    }
}
void EventQA::stop(){
    status = status_stopping;
    mQAThread->join();
    delete TServ;
}
void EventQA::run(){}

string EventQA::getRawEventFileName(){
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, rawEventFilePrefix.size()) != rawEventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(rawEventFilePrefix.size(),name.size() - suffix.size()));
        if(id>currentRawEventFileID){
            currentRawEventFileID = id;
        }
    }
    return dir + rawEventFilePrefix + to_string(currentRawEventFileID) + ".root";
}
string EventQA::getEventFileName(){
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, eventFilePrefix.size()) != eventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(eventFilePrefix.size(),name.size() - suffix.size()));
        if(id>currentEventFileID){
            currentEventFileID = id;
        }
    }
    return dir + eventFilePrefix + to_string(currentEventFileID) + ".root";
}
void EventQA::setTHttpServerPort(int port){
    THttpServerPort = port;
}
void EventQA::setDir(const char* _dir){
    dir = _dir;
    if(dir[dir.length()-1]!='/')dir.append("/");
    std::filesystem::create_directory(dir.c_str());
}
string EventQA::get(const char* name){
    if (strcasecmp(name, "track_2D_ZX") == 0){
        lock.lock();
        string str = TBufferJSON::ToJSON(track_2D_ZX).Data();
        lock.unlock();
        return str;
    }
    if(strcasecmp(name, "track_2D_ZY") == 0){
        lock.lock();
        string str = TBufferJSON::ToJSON(track_2D_ZY).Data();
        lock.unlock();
        return str;
    }
    if (strcasecmp(name, "track_2D_XY") == 0){
        lock.lock();
        string str = TBufferJSON::ToJSON(track_2D_XY).Data();
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
    if (strcasecmp(name, "waveform") == 0){
        lock.lock();
        string str =  TBufferJSON::ToJSON(mg).Data();
        lock.unlock();
        return str;
    }

    return string("");
}
string EventQA::getList(){
    string list = "";
    list += "track_2D_ZX";
    list += "\t";
    list += "track_2D_ZY";
    list += "\t";
    list += "track_2D_XY";
    list += "\t";
    list += "track_3D";
    list += "\t";
    list += "Pad_ADC";
    list += "\t";
    list += "mesh_energy";
    list += "\t";
    list += "mesh_adc";
    list += "\t";
    list += "waveform";
    list += "\t";
    return list;
}

void EventQA::mQALoop(){
    status = status_running;
    int finishedFileID = 0;
    totalEvent = 0;
    RawEvent *rawEvent;
    Event *event;
    while(status == status_running){
        string fileName = getRawEventFileName();
        if(finishedFileID == currentRawEventFileID){
            sleep(0.1);
            continue;
        }
        TFile file(fileName.c_str());
        TTree* tree;
        file.GetObject("rawEvent",tree);
        tree->SetBranchAddress("rawEvent",&rawEvent);
        int nentries = tree->GetEntriesFast();
        for(int jentry=0;jentry<nentries;jentry++){
            tree->GetEntry(jentry);
            totalEvent++;
            currentEventID = rawEvent->event_id;
            event = converter.convert(*rawEvent);
            //---------------------------------
            // Do QA with the event here
            //---------------------------------
            lock.lock();
            fill(*rawEvent, *event);
            lock.unlock();
        }
        finishedFileID = currentRawEventFileID;
    }
    status = status_stopped;
}
void EventQA::updateSettings(const char* msg){
  converter.updateSettings(msg);
}

void EventQA::fill(const RawEvent &revt, const Event &evt){
    // cout<<"QA Event ID "<<evt.event_id<<endl;
    currentEventID = evt.event_id;

    int time_x[1024]; //ns, 25ns per bin
    for(int i=0;i<1024;i++)time_x[i]=i*25;
    
    const int numberofrows = 64;
    const int numberofcols = 32;

    float mesh_energy = 0;
    float mesh_charge = 0;

    double t0_ref = 0;
    TVector3 point(0,0,0);

    track_2D_ZX->Reset();
    track_2D_ZY->Reset();
    track_2D_XY->Reset();
    track_3D->Reset();
    int i = 0;
    for(auto ch = revt.channels.begin(); ch != revt.channels.end(); ++ch){
        for(auto pad = evt.pads.begin(); pad!= evt.pads.end(); pad++){
            int padrow = pad->padRow;
            int padcol = pad->padColumn;
            int adc = pad->adc;
            // int height =int(pad->DriftTime*(evt.Vdrift));
            int charge = pad->ChargeDeposited;
            mesh_energy += pad->Energy;
            mesh_charge += charge;

            if(pad->padNumber==pad_numQA)
                Pad_ADC->Fill(adc);
            
            if(i==0)t0_ref = pad->DriftTime; 
            if(padrow<5){point.SetX(-(6*padrow+6./2)+72);}
            else if(5<=padrow&&padrow<9){point.SetX(-(6*5+(padrow-5)*5+5./2)+72);}
            else if(9<=padrow&&padrow<12){point.SetX(-(6*5+5*4+(padrow-9)*4+4./2)+72);}
            else if(12<=padrow&&padrow<14){point.SetX(-(6*5+5*4+4*3+(padrow-12)*3+3./2)+72);}
            else if(14<=padrow&&padrow<18){point.SetX(-(6*5+5*4+4*3+3*2+(padrow-14)*2+2./2)+72);}
            else if(18<=padrow&&padrow<20){point.SetX(-(6*5+5*4+4*3+3*2+2*4+(padrow-18)*3+3./2)+72);}
            else if(20<=padrow&&padrow<23){point.SetX(-(6*5+5*4+4*3+3*2+2*4+3*2+(padrow-20)*4+4./2)+72);}
            else if(23<=padrow&&padrow<27){point.SetX(-(6*5+5*4+4*3+3*2+2*4+3*2+4*3+(padrow-23)*5+5./2)+72);}
            else {point.SetX(-(6*5+5*4+4*3+3*2+2*4+3*2+4*3+5*4+(padrow-27)*6+6./2)+72);}
            point.SetZ(padcol*4.5+2.25-144);//mm
            point.SetY((t0_ref-pad->DriftTime)*(evt.Vdrift)); //mm
            if(point.z()<-90||point.z()>90)continue;

            track_2D_ZX->Fill(point.z(),point.x(),charge);
            track_2D_ZY->Fill(point.z(),point.y(),charge);
            track_2D_XY->Fill(point.x(),point.y(),charge);
            track_3D->Fill(point.z(),point.x(),point.y(),charge);
        }
        if(gr[i]!=NULL){
            mg->RecursiveRemove(gr[i]);
            gr[i]->Delete();
            gr[i]=NULL;
        }
        gr[i] = new TGraph(1024,time_x,ch->waveform);
        gr[i]->GetXaxis()->SetTitle("Time (ns)");
        gr[i]->GetYaxis()->SetTitle("ADC");
        // gr[i]->SetName(Form( "event-%li: raw-%i col-%i",evt.event_id,padrow,padcol));
        // gr[i]->SetTitle(Form("event-%li: raw-%i col-%i",evt.event_id,padrow,padcol));
        gr[i]->SetName(Form( "channel-%i:%i",ch->channel_id,i));
        gr[i]->SetTitle(Form("channel-%i:%i",ch->channel_id,i));
        mg->Add(gr[i],"PL");
        i++;
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
