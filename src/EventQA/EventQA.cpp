#include "EventQA.h"
#include <iostream>
#include "TRandom3.h"
#include "TFile.h"
#include "TList.h"
#include "TIterator.h"
#include "TColor.h"

ClassImp(EventQA);

EventQA::EventQA(int port){
    THttpServerPort = port;
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
    track_2D->GetXaxis()->SetTitle("Z(mm)");
    track_2D->GetYaxis()->SetTitle("X(mm)");
    track_3D = new TH3D("track_3D","track_3D",ZNbins,ZEdge,XNbins,XEdge,YNbins,YEdge);
    track_3D->GetXaxis()->SetTitle("Z(mm)");
    track_3D->GetYaxis()->SetTitle("X(mm)");
    track_3D->GetZaxis()->SetTitle("Y(mm)");
    setpad_numQA(16,32);
    totalEvent = 0;
    currentEventID = 0;
    cout<<"initEventQA\n";
}
EventQA::~EventQA(){
    
}
void EventQA::setDir(const char* _dir){
    dir = _dir;
    if(dir[dir.length()-1]!='/')dir.append("/");
    std::filesystem::create_directory(dir.c_str());
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
    list += "track_2D";
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
void EventQA::setMessageHost(int port, const char* host){
    socketPort = port;
    socketHost = host;
}
void EventQA::stop(){
    status = status_stopping;
    mQAThread->join();
    mTServThread->join();
    status = status_stopped;
}
void EventQA::run(){
    status = status_running;
    mQAThread = new thread(&EventQA::mQALoop, this);
    mTServThread = new thread(&EventQA::mTServLoop, this);
}
// float EventQA::getRate(){
//     now.Set();
//     elapsed = now - start_time;
//     double t = static_cast<double>(elapsed.AsDouble());
//     if(t>6) rate =0;
//     return rate;
// }
void EventQA::mTServLoop(){
    cout<<"THttp Server start"<<endl;
    TServ = new THttpServer(Form("http:%d",THttpServerPort));
    TServ->SetReadOnly(kFALSE);
     while(status == status_running){
        TServ->ProcessRequests();
        usleep(100000);
    }
    delete TServ;
    cout<<"THttp Server stop"<<endl;
}
void EventQA::mQALoop(){
    TMessageSocket *socket = new TMessageSocket(socketPort, socketHost.c_str());
    // int rateCount = 0;
    // int nEvents4Rate = 1;
    cout<<"Event QA loop start"<<endl;
    totalEvent = 0;
    while(status == status_running){
        TMessage *msg = socket->getMessage();
        if(msg == NULL){
            sleep(0.1);
            continue;
        }
        if(msg->What() != kMESS_OBJECT) continue;
        totalEvent++;
        RawEvent *revt = (RawEvent*)msg->ReadObjectAny(RawEvent::Class());
        // rateCount++;
        // if(rateCount >= nEvents4Rate){
        //     now.Set();
        //     elapsed = now - start_time;
        //     double t = static_cast<double>(elapsed.AsDouble());
        //     if(t < 3){
        //         nEvents4Rate++;
        //     }else{
        //         rate = rateCount/t;
        //         start_time.Set();
        //         rateCount = 0;
        //     }
        // }
        Event *evt = converter.convert(*revt);
        //---------------------------------
        // Do QA with the event here
        //---------------------------------
        lock.lock();
        fill(*revt, *evt);
        lock.unlock();
        delete msg;
        delete revt;
    }
    delete socket;
    cout<<"Event QA Loop stop"<<endl;
}
void EventQA::updateSettings(const char* msg){
  converter.updateSettings(msg);
}

void EventQA::fill(const RawEvent &revt, const Event &evt){
    // cout<<"QA Event ID "<<evt.event_id<<endl;
    currentEventID = evt.event_id;
    if(mg!=nullptr)delete mg;
    mg = new TMultiGraph();

    int time_x[1024]; //ns, 25ns per bin
    for(int i=0;i<1024;i++)time_x[i]=i*25;
    
    const int numberofrows = 64;
    const int numberofcols = 32;

    float mesh_energy = 0;
    float mesh_charge = 0;

    track_2D->Reset();
    track_3D->Reset();
    for(int i=0;i<revt.NChannel;i++){
        Channel* ch = (Channel*) revt.channels->At(i);
        for(auto padIter : *(evt.pads)){
            Pad* pad = (Pad*)padIter;
            int padrow = pad->padRow;
            int padcol = pad->padColumn;
            int adc = pad->adc;
            int height =int(pad->DriftTime*(evt.Vdrift));
            // cout<<pad->DriftTime<<"  "<<evt.event.Vdrift<<endl;
            int charge = pad->ChargeDeposited;
            mesh_energy += pad->Energy;
            mesh_charge += charge;
            // cout<<"pad->Energy: "<<pad->Energy<<endl;

            if(pad->padNumber==pad_numQA)
                Pad_ADC->Fill(adc);
            // track_2D->SetBinContent(padcol+1,padrow+1,adc);
            // track_3D->SetBinContent(padcol+1,padrow+1,180-height,adc);
            track_2D->SetBinContent(padcol+1,padrow+1,charge);
            //track_3D->SetBinContent(padcol+1,padrow+1,180-height,charge); 
            track_3D->SetBinContent(padcol+1,padrow+1,90,charge); 
            
            TGraph* gr = new TGraph(1024,time_x,ch->waveform);
            gr->GetXaxis()->SetTitle("Time (ns)");
            gr->GetYaxis()->SetTitle("ADC");
            gr->SetName(Form( "event-%li: raw-%i col-%i",evt.event_id,padrow,padcol));
            gr->SetTitle(Form("event-%li: raw-%i col-%i",evt.event_id,padrow,padcol));
            gr->SetLineColor(colorindex[i%12]);
            // for(int j=0;j<1024;j++){
            //     unsigned int waveform = ch->waveform[j];
            //     gr->SetPoint(j,j*25,waveform);
            // }
            // cout<<"fill graph"<<endl;
            mg->Add(gr,"PL");
        }
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
