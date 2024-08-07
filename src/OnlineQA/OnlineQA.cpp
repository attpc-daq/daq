#include "OnlineQA.h"
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

OnlineQA::OnlineQA(int id){
    ROOT::EnableThreadSafety();
    keyID = id;
    key_t shmkey = ftok(".", keyID);
    shmid = shmget(shmkey, sizeof(struct shmseg)+256, 0644|IPC_CREAT);
    if (shmid == -1) {
      perror("Online QA Shared memory error");
    }
    shmp = (struct shmseg*) (shmat(shmid, NULL, 0));
    if (shmp == (void *) -1) {
      perror("Online Shared memory attach error");
    }
    shmp->status = 0;
    event_id = new TH1I("event id","event_id",1000,0,1000);

    // Pad_ADC = new TH1D("Pad_ADC","Pad_ADC",4096,0,4095);//ADC码值
    // Pad_ADC->GetXaxis()->SetTitle("channels");
    // Pad_ADC->GetYaxis()->SetTitle("counts");
    event_time = new TH1I("trigger rate","trigger rate",250,0,1000);//unit: us
    event_time->GetXaxis()->SetTitle("timestamp (us)");
    event_time->GetYaxis()->SetTitle("rawEvent counts");
    // Mesh_Energy_Spectrum = new TH1D("Mesh_Energy_Spectrum","Mesh_Energy_Spectrum",1000,0,10);//unit: MeV
    // Mesh_Energy_Spectrum->GetXaxis()->SetTitle("energy(MeV)");
    // Mesh_Energy_Spectrum->GetYaxis()->SetTitle("counts");
    // Mesh_ADC_Spectrum = new TH1D("Mesh_ADC_Spectrum","Mesh_ADC_Spectrum",4096,0,4095);
    // Mesh_ADC_Spectrum->GetXaxis()->SetTitle("channels");
    // Mesh_ADC_Spectrum->GetYaxis()->SetTitle("counts");
    // // Mesh_charge_Spectrum = new TH1D("Mesh_charge_Spectrum","Mesh_charge_Spectrum",250,0,100);//unit: pC
    // Mesh_charge_Spectrum = new TH1D("Mesh_charge_Spectrum","Mesh_charge_Spectrum",500,0,1);//unit: pC
    // Mesh_charge_Spectrum->GetXaxis()->SetTitle("charge (pc)");
    // Mesh_charge_Spectrum->GetYaxis()->SetTitle("counts");
    // const Int_t XNbins=32;
    // Double_t XEdge[XNbins+1]={0};
    // for(int i=0;i<=XNbins;i++){
    //     if(i<=5)XEdge[i]=6*i-72;
    //     else if(5<i&&i<=9)XEdge[i]=6*5+(i-5)*5-72;
    //     else if(9<i&&i<=12)XEdge[i]=6*5+5*4+(i-9)*4-72;
    //     else if(12<i&&i<=14)XEdge[i]=6*5+5*4+4*3+(i-12)*3-72;
    //     else if(14<i&&i<=18)XEdge[i]=6*5+5*4+4*3+3*2+(i-14)*2-72;
    //     else if(18<i&&i<=20)XEdge[i]=6*5+5*4+4*3+3*2+2*4+(i-18)*3-72;
    //     else if(20<i&&i<=23)XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+(i-20)*4-72;
    //     else if(23<i&&i<=27)XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+4*3+(i-23)*5-72;
    //     else XEdge[i]=6*5+5*4+4*3+3*2+2*4+3*2+4*3+5*4+(i-27)*6-72;
    // }
    
    // const Int_t ZNbins=64;
    // Double_t ZEdge[ZNbins+1]={0};
    // for(int i=0;i<=ZNbins;i++){ZEdge[i]=i*4.5-144.;}

    // const Int_t YNbins=180;
    // Double_t YEdge[YNbins+1]={0};
    // for(int i=0;i<=YNbins;i++){YEdge[i]=i*1.0-90;}

    // track_2D_ZX = new TH2D("track_2D_ZX","track_2D_ZX",ZNbins,ZEdge,XNbins,XEdge);
    // track_2D_ZX->GetXaxis()->SetTitle("Z [mm]");
    // track_2D_ZX->GetYaxis()->SetTitle("X [mm]");
    // track_2D_ZY = new TH2D("track_2D_ZY","track_2D_ZY",ZNbins,ZEdge,YNbins,YEdge);
    // track_2D_ZY->GetXaxis()->SetTitle("Z [mm]");
    // track_2D_ZY->GetYaxis()->SetTitle("Y [mm]");
    // track_2D_XY = new TH2D("track_2D_XY","track_2D_XY",XNbins,XEdge,YNbins,YEdge);
    // track_2D_XY->GetXaxis()->SetTitle("X [mm]");
    // track_2D_XY->GetYaxis()->SetTitle("Y [mm]");
    // track_3D = new TH3D("track_3D","track_3D",ZNbins,ZEdge,XNbins,XEdge,YNbins,YEdge);
    // track_3D->GetXaxis()->SetTitle("Z [mm]");
    // track_3D->GetYaxis()->SetTitle("X [mm]");
    // track_3D->GetZaxis()->SetTitle("Y [mm]");

    // mg = new TMultiGraph("waveform","waveform");
    // mg->GetXaxis()->SetTitle("Time (ns)");
    // mg->GetYaxis()->SetTitle("ADC");
    // for(int i=0;i<2048;i++){
    //     gr[i] = NULL;
    // }
    // setpad_numQA(0,1);
}
OnlineQA::~OnlineQA(){
    shmctl(shmid, IPC_RMID, NULL);
}
void OnlineQA::resetSHM(){
    shmp->status = 0;
    shmp->totalEvent = 0;
    shmp->currentEventID = 0;
    shmp->clearPlots = 0;
}
void OnlineQA::start(){
    if(shmp->status == status_not_started){
        shmp->status = status_starting;
        runPID = fork();
        if(runPID == 0){
            char *path = getenv("PATH");
            char  pathenv[strlen(path) + sizeof("PATH=")];
            sprintf(pathenv, "PATH=%s", path);
            char *envp[] = {pathenv, NULL};
            char *const args[] = {(char*)"OnlineQAExec", (char*)to_string(keyID).c_str(), NULL};
            execvpe("OnlineQAExec", args, envp);
        }
        while(shmp->status != status_running){
            sleep(1);
        }
    }
}
void OnlineQA::stop(){
    shmp->status = status_stopping;
    if(runPID >0){
        int status;
        waitpid(runPID, &status, 0);
        if(WIFEXITED(status)){
            // cout<<" Online QA RUN process exited with status: "<< WEXITSTATUS(status) << std::endl;
        }else{
            std::cerr << "Online QA RUN process terminated abnormally." << std::endl;
        }
    }
}
void OnlineQA::run(){
    ROOT::EnableThreadSafety();
    shmp->status = status_running;
    thread* mDataReceiver = new thread(&OnlineQA::dataReceiver, this);
    thread* mTServLoop = new thread(&OnlineQA::TServLoop, this);

    mDataReceiver->join();
    mTServLoop->join();
    shmp->status = status_stopped;
}
void OnlineQA::TServLoop(){
    int port = shmp->THttpServerPort;
    THttpServer* TServ = new THttpServer(Form("http:%d",port));
    TServ->SetReadOnly(kFALSE);
     while(shmp->status == status_running){
        TServ->ProcessRequests();
        if(shmp->clearPlots == 1){
            //Pad_ADC->Reset(); //清除累计的plots
            event_time->Reset();
            event_id->Reset();
            //
            shmp->clearPlots = 0;
        }
        usleep(100000);
    }
    delete TServ;
}
void OnlineQA::dataReceiver(){
    string host = shmp->dataHost;
    int port = shmp->dataPort;
    AutoSocket* autoSocket= new AutoSocket(port,host.c_str());
    while(shmp->status == status_running){
        TObject * obj = autoSocket->get(RawEvent::Class());
        if(obj == NULL){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        else{
            RawEvent* rawEvent = (RawEvent*)obj;
            shmp->currentEventID = rawEvent->event_id;
            shmp->totalEvent++;
            fill(rawEvent);
            delete obj;
        }
    }
}
void OnlineQA::setDataPort(int port,const char* host){
    shmp->dataPort=port;
    memset(shmp->dataHost, 0, sizeof(shmp->dataHost));
    strncpy(shmp->dataHost, host, strlen(host));
}
void OnlineQA::setTHttpServerPort(int port){
    shmp->THttpServerPort = port;
}
void OnlineQA::clearPlots(){
    shmp->clearPlots = 1;
}

string OnlineQA::get(const char* name){
//     if (strcasecmp(name, "track_2D_ZX") == 0){
//         lock.lock();
//         string str = TBufferJSON::ToJSON(track_2D_ZX).Data();
//         lock.unlock();
//         return str;
//     }
//     if(strcasecmp(name, "track_2D_ZY") == 0){
//         lock.lock();
//         string str = TBufferJSON::ToJSON(track_2D_ZY).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "track_2D_XY") == 0){
//         lock.lock();
//         string str = TBufferJSON::ToJSON(track_2D_XY).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "track_3D") == 0){
//         lock.lock();
//          string str = TBufferJSON::ToJSON(track_3D).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "Pad_ADC") == 0){
//         lock.lock();
//         string str =  TBufferJSON::ToJSON(Pad_ADC).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "trigger_rate") == 0){
//         lock.lock();
//         string str =  TBufferJSON::ToJSON(event_time).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "mesh_energy") == 0){
//         lock.lock();
//         string str =  TBufferJSON::ToJSON(Mesh_Energy_Spectrum).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "mesh_adc") == 0){
//         lock.lock();
//         string str =  TBufferJSON::ToJSON(Mesh_ADC_Spectrum).Data();
//         lock.unlock();
//         return str;
//     }   
//     if (strcasecmp(name, "mesh_charge") == 0){
//         lock.lock();
//         string str =  TBufferJSON::ToJSON(Mesh_charge_Spectrum).Data();
//         lock.unlock();
//         return str;
//     }
//     if (strcasecmp(name, "waveform") == 0){
//         lock.lock();
//         string str =  TBufferJSON::ToJSON(mg).Data();
//         lock.unlock();
//         return str;
//     }
    return string("");
}
string OnlineQA::getList(){
    string list = "";
//     list += "track_2D_ZX";
//     list += "\t";
//     list += "track_2D_ZY";
//     list += "\t";
//     list += "track_2D_XY";
//     list += "\t";
//     list += "track_3D";
//     list += "\t";
//     list += "Pad_ADC";
//     list += "\t";
//     list += "mesh_energy";
//     list += "\t";
//     list += "mesh_adc";
//     list += "\t";
//     list += "mesh_charge";
//     list += "\t";
//     list += "waveform";
//     list += "\t";
    return list;
}

void OnlineQA::updateSettings(const char* msg){
  converter.updateSettings(msg);
}

void OnlineQA::fill(RawEvent *revt, Event* evt){
    Event * event = converter.convert(*revt);
    event_time->Fill(revt->channels[0].timestamp*8.33/1000.);
    event_id->Fill(revt->event_id);
//     currentEventID = event->event_id;
    
//     int time_x[1024]; //ns, 25ns per bin
//     for(int i=0;i<1024;i++)time_x[i]=i*25;
    
//     const int numberofrows = 64;
//     const int numberofcols = 32;

//     double mesh_energy = 0;
//     double mesh_charge = 0;

//     double t0_ref = 0;
//     TVector3 point(0,0,0);

//     track_2D_ZX->Reset();
//     track_2D_ZY->Reset();
//     track_2D_XY->Reset();
//     track_3D->Reset();
//     int i = 0;
    
//     //计算所有pad->DriftTime的平均值作为t0_ref
//     for(auto pad = event->pads.begin(); pad!= event->pads.end(); pad++){
//         t0_ref += pad->DriftTime;
//     }
//     t0_ref = t0_ref/event->pads.size();
    
//     for(auto pad = event->pads.begin(); pad!= event->pads.end(); pad++){
//         // if(event->pads.size()>9||event->pads.size()<2)continue;
// 	    int padrow = pad->padRow;
//             int padcol = pad->padColumn;
//             int adc = pad->adc;
//             // int height =int(pad->DriftTime*(evt.Vdrift));
//             double charge = pad->ChargeDeposited;
//             mesh_energy += pad->Energy;
//             mesh_charge += charge;

//             if(pad->padNumber==pad_numQA)
//                 Pad_ADC->Fill(adc);
            
//             if(padrow<5){point.SetX(-(6*padrow+6./2)+72);}
//             else if(5<=padrow&&padrow<9){point.SetX(-(6*5+(padrow-5)*5+5./2)+72);}
//             else if(9<=padrow&&padrow<12){point.SetX(-(6*5+5*4+(padrow-9)*4+4./2)+72);}
//             else if(12<=padrow&&padrow<14){point.SetX(-(6*5+5*4+4*3+(padrow-12)*3+3./2)+72);}
//             else if(14<=padrow&&padrow<18){point.SetX(-(6*5+5*4+4*3+3*2+(padrow-14)*2+2./2)+72);}
//             else if(18<=padrow&&padrow<20){point.SetX(-(6*5+5*4+4*3+3*2+2*4+(padrow-18)*3+3./2)+72);}
//             else if(20<=padrow&&padrow<23){point.SetX(-(6*5+5*4+4*3+3*2+2*4+3*2+(padrow-20)*4+4./2)+72);}
//             else if(23<=padrow&&padrow<27){point.SetX(-(6*5+5*4+4*3+3*2+2*4+3*2+4*3+(padrow-23)*5+5./2)+72);}
//             else {point.SetX(-(6*5+5*4+4*3+3*2+2*4+3*2+4*3+5*4+(padrow-27)*6+6./2)+72);}
//             point.SetZ(padcol*4.5+2.25-144);//mm
//             point.SetY((t0_ref-pad->DriftTime)*(event->Vdrift)); //mm
//             // cout<<"t0_ref: "<<t0_ref<<" pad->DriftTime: "<<pad->DriftTime<<" event->Vdrift: "<<event->Vdrift<<" height: "<<(t0_ref-pad->DriftTime)*(event->Vdrift)<<endl;

//             track_2D_ZX->Fill(point.z(),point.x(),charge);
//             track_2D_ZY->Fill(point.z(),point.y(),charge);
//             track_2D_XY->Fill(point.x(),point.y(),charge);
//             track_3D->Fill(point.z(),point.x(),point.y(),charge);
//     }

//     for(int j=0;j<2048;j++){
//         if(gr[j]!=NULL){
//             mg->RecursiveRemove(gr[j]);
//             gr[j]->Delete();
//             gr[j]=NULL;
//         }
//     }
//     for(auto ch = rawEvent->channels.begin(); ch != rawEvent->channels.end(); ++ch){
//         // if(gr[i]!=NULL){
//         //     mg->RecursiveRemove(gr[i]);
//         //     gr[i]->Delete();
//         //     gr[i]=NULL;
//         // }
//         gr[i] = new TGraph(1024,time_x,ch->waveform);
//         gr[i]->GetXaxis()->SetTitle("Time (ns)");
//         gr[i]->GetYaxis()->SetTitle("ADC");
//         // gr[i]->SetName(Form( "event-%li: raw-%i col-%i",evt.event_id,padrow,padcol));
//         // gr[i]->SetTitle(Form("event-%li: raw-%i col-%i",evt.event_id,padrow,padcol));
//         gr[i]->SetName(Form( "channel-%i:%i",ch->channel_id,i));
//         gr[i]->SetTitle(Form("channel-%i:%i",ch->channel_id,i));
//         mg->Add(gr[i],"PL");
//         i++;
//     }

//     event_time->Fill(rawEvent->channels[0].timestamp*8.33/1000.);
//     // cout<<"=================>>>mesh_energy: "<<mesh_energy<<endl;
//     Mesh_charge_Spectrum->Fill(mesh_charge*1E+15/6.24150975E+18/1E+3);//mesh_charge*1E+15/6.24150975E+18 unit:fc
//     Mesh_ADC_Spectrum->Fill(mesh_charge*1E+15/6.24150975E+18*0.75/10*4096/4000);//mesh_charge*1E+15/6.24150975E+18 unit:fc  假设ADC量程为4V=4000mV，精度为12bit  主放增益为10，前放增益为0.75mv/fc
//     Mesh_Energy_Spectrum->Fill(mesh_energy);//unit: MeV
}

void OnlineQA::setpad_numQA(long num){
//     Pad_ADC->Reset();
//     pad_numQA = num;
}

void OnlineQA::setpad_numQA(long row, long column){
//     //row: 0~31 column: 0~63
//     Pad_ADC->Reset();
//     pad_numQA = row*64+column;
}
