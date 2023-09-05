#include "DataProcessor.h"
#include <iostream>
#include "TRandom3.h"
#include "TFile.h"
#include <TServerSocket.h>
#include <TSocket.h>
#include <TMonitor.h>
#include <TTimeStamp.h>

ClassImp(DataProcessor);

DataProcessor::DataProcessor(){
    for(int i=0;i<1000;i++){
        rawEventBuffer.push_back(new RawEvent());
        BufferMark[i]=0;
    }
    setDir();
    setDataPort();
    setQAPort();
    setFileEvents();
    setRawEventFilePrefix();
    setRawEventTreeName();
    setRawEventBranchName();
    setEventFilePrefix();
    setEventTreeName();
    setEventBranchName();
    setRawEventSave();
    setEventSave();
    setQA();
    // rate = 0;
    WValue = 30.0;//unit: eV
    Vdrift = 10.0;//unit: mm/ns
    FPC2 = {
        {{"0", 15}},
        {{"1", 15}},
        {{"2", 15}},
        {{"3", 15}},
        {{"4", 15}},
        {{"5", 15}},
        {{"6", 15}},
        {{"7", 15}},
        {{"8", 15}},
        {{"9", 15}},
        {{"10", 15}},
        {{"11", 15}},
        {{"12", 15}},
        {{"13", 15}},
        {{"14", 15}},
        {{"15", 15}},
        {{"16", 15}},
        {{"17", 15}},
        {{"18", 15}},
        {{"19", 15}},
        {{"20", 15}},
        {{"21", 15}},
        {{"22", 15}},
        {{"23", 15}},
        {{"24", 15}},
        {{"25", 15}},
        {{"26", 15}},
        {{"27", 15}},
        {{"28", 15}},
        {{"29", 15}},
        {{"30", 15}},
        {{"31", 15}}
    };
    cout<<"initdataprocessor\n";
    ss = NULL;
    rawEventCount = 0;
    eventCount = 0;
    nMakePar = -1;
    rawEventFileID =0;
    totalEvent = 0;
    currentEventID = 0;
}
DataProcessor::~DataProcessor(){}
void DataProcessor::setDir(const char* _dir){
    dir = _dir;
    if(dir[dir.length()-1]!='/')dir.append("/");
    std::filesystem::create_directory(dir.c_str());
}
void DataProcessor::setDataPort(int port){
    dataPort = port;
}
void DataProcessor::setQAPort(int port){
    QAPort = port;
}
void DataProcessor::setFileEvents(uint64_t n){
    nEvents = n;
}
void DataProcessor::setRawEventFilePrefix(const char* prefix){
    rawEventFilePrefix = prefix;
}
void DataProcessor::setRawEventTreeName(const char* name){
    rawEventTreeName = name;
}
void DataProcessor::setRawEventBranchName(const char* name){
    rawEventBranchName = name;
}
void DataProcessor::setEventFilePrefix(const char* prefix){
    eventFilePrefix = prefix;
}
void DataProcessor::setEventTreeName(const char* name){
    eventTreeName = name;
}
void DataProcessor::setEventBranchName(const char* name){
    eventBranchName = name;
}

void DataProcessor::setRawEventSave(bool k){
    kRawEventSave = k;
}
void DataProcessor::setEventSave(bool k){
    kEventSave = k;
}
void DataProcessor::setQA(bool k){
    if(!k){
        if(QASocket!=NULL){
            delete QASocket;
            QASocket = NULL;
        }
    }
    kQA = k;
}
// float DataProcessor::getRate(){
//     now.Set();
//     elapsed = now - start_time;
//     double t = static_cast<double>(elapsed.AsDouble());
//     if(t>6) rate =0;
//     return rate;
// }
void DataProcessor::generateParameters(int n){
    parameter.reset();
    nMakePar = n;
}
string DataProcessor::getRawEventFileList(int n){
    string list;
    int count = 0;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, rawEventFilePrefix.size()) != rawEventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        list.append(file.path());
        list.append(";");
        count ++;
        if(n>0&&count>n)break;
    }
    return list;
}
string DataProcessor::getEventFileList(int n){
    string list;
    int count = 0;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, eventFilePrefix.size()) != eventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        list.append(file.path());
        list.append(";");
        count ++;
        if(n>0&&count>n)break;
    }
    return list;
}

void DataProcessor::stop(){
    status = status_stopping;
    cout<<"Dataprocessor stopping"<<endl;
    mThread->join();
    cout<<"DataProcessor stop"<<endl;
}
void DataProcessor::run(){
    status = status_starting;
    cout<<"DataProcessor start"<<endl;
    mThread = new thread(&DataProcessor::loop, this);
}

void DataProcessor::loop(){
    converter = NULL;
    QASocket = NULL;
    eventCount = 0;
    rawEventCount = 0;
    updateEventFileID();
    updateRawEventFileID();
    ss = new TServerSocket(dataPort, true);
    ss->SetOption(kNoBlock, 1);
    TSocket* sc;
    serverMonitor.Add(ss);
    status = status_running;
    
    start_time.Set();
    // rateCount = 0;
    // int nEvents4Rate = 400;
    int clientnumber=0;
    while(status == status_running){
        sc = clientMonitor.Select(1);
        if(sc == (TSocket *)-1){
            // cout<<"no decoder client active"<<endl;
            serverMonitor.ResetInterrupt();
            TSocket* s = serverMonitor.Select(1);
            if(s == (TSocket *)-1) {
                sleep(1);
                continue;
            }
            sc = ((TServerSocket *)s)->Accept();
            if(sc == (TSocket *)-1) {
                sleep(1);
                continue;
            }
            clientMonitor.Add(sc);
            clientnumber++;
            // cout<<"clients "<<clientnumber<<endl;
        }
        TMessage* msg;
        int n = sc->Recv(msg);
        if(n<=0)continue;
        RawEvent* revt = (RawEvent*)msg->ReadObjectAny(RawEvent::Class());
        // if(ptr == NULL) continue;
        // RawEvent* revt =(RawEvent*) ptr;
        int idInBuffer = revt->event_id%1000;
        if(BufferMark[idInBuffer]==0){
            *rawEventBuffer[idInBuffer] = *revt;
            BufferMark[idInBuffer]=1;
        }else{
            if(rawEventBuffer[idInBuffer]->Add(revt)){
                BufferMark[idInBuffer]=2;
            }else{
                *rawEventBuffer[idInBuffer] = *revt;
                BufferMark[idInBuffer]=1; 
            }
        }
        delete revt;
        if(BufferMark[idInBuffer]==2){
            totalEvent++;
            currentEventID = rawEventBuffer[idInBuffer]->event_id;
            if(kRawEventSave)saveRawEvent(rawEventBuffer[idInBuffer]);
            if(kEventSave){
                saveEvent(rawEventBuffer[idInBuffer]);
            }else if (converter != NULL){
                delete converter;
                converter = NULL;
            }
            if(nMakePar>0)makePar(rawEventBuffer[idInBuffer]);
            if(kQA){
                QA(rawEventBuffer[idInBuffer]);
            }else if(QASocket!=NULL){
                delete QASocket;
                QASocket = NULL;
            }
            BufferMark[idInBuffer]=0;
        }
        delete msg;
    }
    for(auto ptr:*(clientMonitor.GetListOfActives())){
        // cout<<"waiting for client to close"<<endl;
        TSocket* sc = (TSocket*)ptr;
        clientMonitor.Remove(sc);
        sc->Close();
    }

    ss->Close();
    if(QASocket != NULL) {
        delete QASocket;
        QASocket = NULL;
    }
    if(eventCount>0)closeEventFile();
    if(rawEventCount>0)closeRawEventFile();
    status = status_stopped;
}
void DataProcessor::saveRawEvent(RawEvent* revt){
    if(rawEventCount == 0){
        createRawEventFile();
    }
    rawEvent = *revt;
    rawEventTree->Fill();
    rawEventCount++;
    if(rawEventCount == nEvents){
        rawEventCount = 0;
        closeRawEventFile();
    }
}
void DataProcessor::saveEvent(RawEvent* revt){
    if(eventCount == 0){
        createEventFile();
    }
    if(converter == NULL){
        converter = new EventConverter((dir+"eventParameters.json").c_str());
    }
    event = *(converter->convert(*revt));
    eventTree->Fill();
    eventCount++;
    if(eventCount == nEvents){
        eventCount = 0;
        closeEventFile();
    }
}
void DataProcessor::makePar(RawEvent* revt){
    parameter.fill(revt);
    nMakePar--;
    if(nMakePar == 0){
        ofstream file1((dir+"thresholdes.json").c_str());
        file1<<parameter.getThreshold();
        file1.close();
        ofstream file2((dir+"eventParameters.json").c_str());
        // file2<<parameter.getSettings(WValue,Vdrift);
        file2<<parameter.getSettings(WValue,Vdrift,FPC2,ElectronicFile,MicromegasFile);
        file2.close();
        // cout<<"parameter generations done"<<endl;
    }
}
void DataProcessor::QA(RawEvent* revt){
    if(QASocket==NULL){
        QASocket = new TMessageSocket(QAPort);
    }
    // cout<<"DataProcessor::QA "<<revt->event_id<<endl;
    QASocket->put(revt);
}
void DataProcessor::closeEventFile(){
    eventFile->cd();
    eventFile->Write();
    eventFile->Close();
}
void DataProcessor::createEventFile(){
    eventFileID++;
    eventFile = new TFile((dir+eventFilePrefix+to_string(eventFileID)+".root").c_str(), "RECREATE");
    eventFile->cd();
    eventTree = new TTree(eventTreeName.c_str(), eventTreeName.c_str());
    eventTree->Branch(eventBranchName.c_str(), &event);

    // cout<<"event file created:"<<(dir+eventFilePrefix+to_string(eventFileID)+".root")<<endl;
}
void DataProcessor::closeRawEventFile(){
    rawEventFile->cd();
    rawEventFile->Write();
    rawEventFile->Close();
}
void DataProcessor::createRawEventFile(){
    rawEventFileID++;
    rawEventFile = new TFile((dir+rawEventFilePrefix+to_string(rawEventFileID)+".root").c_str(), "RECREATE");
    rawEventFile->cd();
    rawEventTree = new TTree(rawEventTreeName.c_str(), rawEventTreeName.c_str());
    rawEventTree->Branch(rawEventBranchName.c_str(), &rawEvent);
    
    // cout<<"raw event file created:"<<(dir+rawEventFilePrefix+to_string(rawEventFileID)+".root")<<endl;
}
void DataProcessor::updateEventFileID(){
    eventFileID = 0;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, eventFilePrefix.size()) != eventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(eventFilePrefix.size(),name.size() - suffix.size()));
        if(id>eventFileID)eventFileID = id;
    }
    // cout<<"event file id:"<<eventFileID<<endl;
}
void DataProcessor::updateRawEventFileID(){
    rawEventFileID = 0;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, rawEventFilePrefix.size()) != rawEventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(rawEventFilePrefix.size(),name.size() - suffix.size()));
        if(id>rawEventFileID)rawEventFileID = id;
    }
    // cout<<"raw event file id:"<<rawEventFileID<<endl;
}

void DataProcessor::setFPC2(std::vector<std::map<string,int>> fpc2){
    std::vector<std::map<string, int>>().swap(FPC2);
    for (const auto& map : fpc2) {
        FPC2.push_back(map);
        // 输出map的键和值
        // for (const auto& pair : map) {
        //     cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
        // }
    }
}