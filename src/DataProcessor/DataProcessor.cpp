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

    nMakePar = -1;
    rawEventFileID =0;
    totalEvent = 0;
    currentEventID = 0;

}
DataProcessor::~DataProcessor(){
}
void DataProcessor::setDir(const char* _dir){
    dir = _dir;
    if(dir[dir.length()-1]!='/')dir.append("/");
    std::filesystem::create_directory(dir.c_str());
}
void DataProcessor::setDataPort(int port1,const char* host1, int port2, const char* host2){
    dataPort1=port1;
    dataHost1=host1;
    dataPort2=port2;
    dataHost2=host2;
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
    if(kQA){
        QASocket = new TMessageSocket(QAPort, nEvents);
    }
}
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

    mMsgThread1->join();
    mMsgThread2->join();
    mAssembleThread->join();
    mBufferProcess->join();
    cout<<"DataProcessor stop"<<endl;
    status = status_stopped;
}
void DataProcessor::run(){
    status = status_running;
    cout<<"DataProcessor start"<<endl;

    mMsgThread1 = new thread(&DataProcessor::msgReceiver, this, dataPort1,dataHost1.c_str(), &bufferDQ01);
    mMsgThread2 = new thread(&DataProcessor::msgReceiver, this, dataPort2,dataHost2.c_str(), &bufferDQ02);
    mAssembleThread = new thread(&DataProcessor::assemble, this);
    mBufferProcess = new thread(&DataProcessor::bufferProcess,this);
}
void DataProcessor::bufferProcess(){
    updateRawEventFileID();
    updateEventFileID();
    outputFileID = eventFileID > rawEventFileID ? eventFileID : rawEventFileID;
    while(status == status_running){
        if(bufferDQ.size()>0){
            outputFileID++;
            new thread(&DataProcessor::rawEventProcess,this,bufferDQ.front(),outputFileID);
            bufferDQ.pop_front();
        }else{
            sleep(1);
        }
    }
    if(QASocket!=NULL)delete QASocket;
}
void DataProcessor::rawEventProcess(RawEvent** ptrArray, int fileID){
    cout<<"outputfileID "<<fileID<<endl;
    RawEvent** rawEventPtrArray = new RawEvent*[nEvents];
    TFile* rawEventFile=NULL;
    TTree* rawEventTree=NULL;
    TFile* eventFile=NULL;
    TTree* eventTree=NULL;
    EventConverter* converter;
    RawEvent rawEvent;
    Event event;

    if(kRawEventSave){
        rawEventFile = new TFile((dir+rawEventFilePrefix+to_string(fileID)+".root").c_str(), "RECREATE");
        rawEventFile->cd();
        rawEventTree = new TTree(rawEventTreeName.c_str(), rawEventTreeName.c_str());
        rawEventTree->Branch(rawEventBranchName.c_str(), &rawEvent);
    }
    if(kEventSave){
        eventFile = new TFile((dir+eventFilePrefix+to_string(fileID)+".root").c_str(), "RECREATE");
        eventFile->cd();
        eventTree = new TTree(eventTreeName.c_str(), eventTreeName.c_str());
        eventTree->Branch(eventBranchName.c_str(), &event);
        converter = new EventConverter((dir+"eventParameters.json").c_str());
    }
    for(int i =0 ;i<nEvents;i++){
        if(ptrArray[i*2]!=NULL && ptrArray[i*2+1]!=NULL){
            rawEventPtrArray[i]=ptrArray[i*2];
            rawEventPtrArray[i]->Add(ptrArray[i*2+1]);
            delete ptrArray[i*2+1];
        }else if(ptrArray[i*2]==NULL && ptrArray[i*2+1]!=NULL){
            rawEventPtrArray[i]=ptrArray[i*2+1];
        }
        else if(ptrArray[i*2]!=NULL && ptrArray[i*2+1]==NULL){
            rawEventPtrArray[i]=ptrArray[i*2];
        }
    }
    for(int i=0;i<nEvents;i++){
        if(rawEventFile!=NULL){  
            rawEvent = *(rawEventPtrArray[i]);
            rawEventTree->Fill();
        }
        if(eventFile!=NULL){
            event = *(converter->convert(*(rawEventPtrArray[i])));
            eventTree->Fill();
        }
    }
    std::unique_lock<std::mutex> lock(mtx);
    for(int i=0;i<nEvents;i++){
        if(QASocket!=NULL){
            QASocket->put(rawEventPtrArray[i]);
        }
        delete rawEventPtrArray[i];
    }
    cv.notify_one();
    if(rawEventFile!=NULL){
        rawEventFile->cd();
        rawEventFile->Write();
        rawEventFile->Close();
    }
    if(eventFile!=NULL){
        eventFile->cd();
        eventFile->Write();
        eventFile->Close();
        delete converter;
    }
    delete rawEventPtrArray;
}
void DataProcessor::msgReceiver(int port, const char* host, deque<RawEvent**> * bufferDQ){
    AutoSocket* autoSocket= new AutoSocket(port,host);
    TMessage* msg;
    RawEvent* revt;
    while(status == status_running){
        if(bufferDQ->size()>10){
            sleep(1);
            continue;
        }
        RawEvent** ptrArray=new RawEvent*[nEvents];
        for(int i=0;i<nEvents;i++){
            while(true){
                if(status != status_running){
                    for(int j=0;j<i;j++){
                        delete ptrArray[j];
                        delete ptrArray;
                        delete autoSocket;
                    }
                    return;
                }
                msg = autoSocket->get();
                if(msg==NULL)continue;
                revt = (RawEvent*)msg->ReadObjectAny(RawEvent::Class());
                delete msg;
                if(!revt)continue;
                break;
            }
            ptrArray[i]=revt;
        }
        bufferDQ->push_back(ptrArray);
    }
    delete autoSocket;
}
void DataProcessor::assemble(){
    RawEvent* revt1=NULL;
    RawEvent* revt2=NULL;
    RawEvent** ptrArray01=NULL;
    RawEvent** ptrArray02=NULL;
    int index1;
    int index2;
    RawEvent** ptrArray=NULL;
    int index;

    while(status == status_running){
        if(ptrArray01==NULL){
            while(bufferDQ01.size()<=0){
                if(status != status_running)return;
                sleep(1);
            }
            ptrArray01 = bufferDQ01.front();
            bufferDQ01.pop_front();
            index1=0;
        }
        if(ptrArray02==NULL){
            while(bufferDQ02.size()<=0){
                if(status != status_running)return;
                sleep(1);
            }
            ptrArray02 = bufferDQ02.front();
            bufferDQ02.pop_front();
            index2=0;
        }
        if(ptrArray01[index1]->event_id == ptrArray02[index2]->event_id){
            revt1 = ptrArray01[index1];
            revt2 = ptrArray02[index2];
            index1++;
            index2++;
	    }else if(ptrArray01[index1]->event_id > ptrArray02[index2]->event_id){
            revt1 = NULL;
            revt2 = ptrArray02[index2];
            index2++;
	    }else if(ptrArray01[index1]->event_id < ptrArray02[index2]->event_id){
            revt1 = ptrArray01[index1];
            revt2 = NULL;
            index1++;
	    }
        if(index1>=nEvents){
            delete ptrArray01;
            ptrArray01=NULL;
        }
        if(index2>=nEvents){
            delete ptrArray02;
            ptrArray02=NULL;
        }
        if(nMakePar>0){
            RawEvent* revt;
            if(revt1!=NULL && revt2!=NULL){
                revt = revt1;
                revt->Add(revt2);
                delete revt2;
            }else if(revt1==NULL){
                revt = revt2;
            }else{
                revt = revt1;
            }
	        makePar(revt);
            delete revt;
	        continue;
        }
        if(bufferDQ.size()>10){//限制最大缓存数，也会影响最大线程数
            sleep(1);
            cout<<" Raw Event Buffer More Than 10, stop receiving"<<endl;
            continue;
        }
        totalEvent++;
        if(ptrArray==NULL){
            ptrArray=new RawEvent*[nEvents*2];
            index=0;
        }
        ptrArray[index*2]=revt1;
        ptrArray[index*2+1]=revt2;
        if(revt1!=NULL){
            currentEventID=revt1->event_id;
        }else{
            currentEventID=revt2->event_id;
        }
        index++;
        if(index>=nEvents){
            bufferDQ.push_back(ptrArray);
            ptrArray=NULL;
        }
        index1++;
        if(index1>=nEvents){
            delete ptrArray01;
            ptrArray01=NULL;
        }
        index2++;
        if(index2>=nEvents){
            delete ptrArray02;
            ptrArray02=NULL;
        }
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
