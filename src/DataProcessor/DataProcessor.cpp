#include "DataProcessor.h"
#include <iostream>
#include "TRandom3.h"
#include "TFile.h"
#include <TServerSocket.h>
#include <TSocket.h>
#include <TMonitor.h>
#include <TTimeStamp.h>
#include <TMessageBufferTP.h>
#include <TROOT.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

DataProcessor::DataProcessor(int id){
    ROOT::EnableThreadSafety();
    keyID = id;
    key_t shmkey = ftok(".", keyID);
    shmid = shmget(shmkey, sizeof(struct shmseg)+256, 0644|IPC_CREAT);
    if (shmid == -1) {
      perror("DataProcessor Shared memory error");
    }
    shmp = (struct shmseg*) (shmat(shmid, NULL, 0));
    if (shmp == (void *) -1) {
      perror("DataProcessor Shared memory attach error");
    }
    shmp->status = 0;
    rawEventFilePrefix = "rawEvent";
    rawEventTreeName = "rawEvent";
    rawEventBranchName = "rawEvent";
    eventFilePrefix = "event";
    eventTreeName = "event";
    eventBranchName = "event";
}
DataProcessor::~DataProcessor(){
    shmctl(shmid, IPC_RMID, NULL);
}
void DataProcessor::resetSHM(){
    shmp->kRawEventSave = false;
    shmp->kEventSave = false;
    shmp->nMakePar = 0;
    shmp->status = 0;
    shmp->totalEvent = 0;
    shmp->currentEventID = 0;
    shmp->outputFileID = 0;
}
void DataProcessor::start(){
    if(shmp->status == status_not_started){
        shmp->status = status_starting;
        runPID = fork();
        if(runPID == 0){
            char *path = getenv("PATH");
            char  pathenv[strlen(path) + sizeof("PATH=")];
            sprintf(pathenv, "PATH=%s", path);
            char *envp[] = {pathenv, NULL};
            char *const args[] = {(char*)"DataProcessorExec", (char*)to_string(keyID).c_str(), NULL};
            execvpe("DataProcessorExec", args, envp);
        }
        while(shmp->status != status_running){
            sleep(1);
        }
    }
}
void DataProcessor::stop(){
    shmp->status = status_stopping;
    if(runPID >0){
        int status;
        waitpid(runPID, &status, 0);
        if(WIFEXITED(status)){
            // cout<<" DataProcessor RUN process exited with status: "<< WEXITSTATUS(status) << std::endl;
        }else{
            std::cerr << "DataProcessor RUN process terminated abnormally." << std::endl;
        }
    }
}

void DataProcessor::run(){
    ROOT::EnableThreadSafety();
    shmp->status = status_running;
    thread* mBufferProcess = new thread(&DataProcessor::bufferProcess,this);
    thread* mMsgThread = new thread(&DataProcessor::msgReceiver, this);
    mMsgThread->join();
    mBufferProcess->join();
    shmp->status = status_stopped;
}

int DataProcessor::getNMakePar(){
    return shmp->nMakePar;
}
int DataProcessor::getOutputFileID(){
    return shmp->outputFileID;
}
int DataProcessor::getTotalEvent(){
    return shmp->totalEvent;
}
int DataProcessor::getCurrentEventID(){
    return shmp->currentEventID;
}
void DataProcessor::setDir(const char* dir){
    memset(shmp->dir, 0, sizeof(shmp->dir));//TODO:
    strncpy(shmp->dir,dir,strlen(dir));
    if(shmp->dir[strlen(dir)-1]!='/')shmp->dir[strlen(dir)]='/';
    std::filesystem::create_directory(shmp->dir);
}
void DataProcessor::setGainFile(const char* E_file,const char* M_file){
    memset(shmp->ElectronicFile, 0, sizeof(shmp->ElectronicFile));//TODO:
    strncpy(shmp->ElectronicFile,E_file,strlen(E_file));
    memset(shmp->MicromegasFile, 0, sizeof(shmp->MicromegasFile));//TODO:
    strncpy(shmp->MicromegasFile,M_file,strlen(M_file));
}
void DataProcessor::setElectronicFile(const char* E_file){
    memset(shmp->ElectronicFile, 0, sizeof(shmp->ElectronicFile));//TODO:
    strncpy(shmp->ElectronicFile,E_file,strlen(E_file));
}
void DataProcessor::setMicromegasFile(const char* M_file){
    memset(shmp->MicromegasFile, 0, sizeof(shmp->MicromegasFile));//TODO:
    strncpy(shmp->MicromegasFile,M_file,strlen(M_file));
}

void DataProcessor::setDataPort(int port1,const char* host1, int port2, const char* host2){
    shmp->dataPort1=port1;
    memset(shmp->dataHost1, 0, sizeof(shmp->dataHost1));//TODO:
    strncpy(shmp->dataHost1, host1, strlen(host1));
    shmp->dataPort2=port2;
    memset(shmp->dataHost2, 0, sizeof(shmp->dataHost2));//TODO:
    strncpy(shmp->dataHost2, host2, strlen(host2));
}

void DataProcessor::setFileEvents(int n){
    shmp->nEvents = n;
}
void DataProcessor::setRawEventSave(bool k){
    shmp->kRawEventSave = k;
}
void DataProcessor::setEventSave(bool k){
    shmp->kEventSave = k;
}
void DataProcessor::generateParameters(int n){
    parameter.reset();
    shmp->nMakePar = n;
}
void DataProcessor::bufferProcess(){
    updateRawEventFileID();
    updateEventFileID();
    shmp->outputFileID = eventFileID > rawEventFileID ? eventFileID : rawEventFileID;
    while(shmp->status == status_running){
        if(rawSubEventBufferDQ.size()>0){
            shmp->outputFileID++;
            int fileID = shmp->outputFileID;
            new thread(&DataProcessor::rawEventProcess,this,rawSubEventBufferDQ.front(),fileID);
            rawSubEventBufferDQ.pop_front();
        }else{
            sleep(1);
        }
    }
}
void DataProcessor::rawEventProcess(RawEvent** ptrArray, int fileID){
    RawEvent** rawEventPtrArray = new RawEvent*[shmp->nEvents];
    TFile* rawEventFile=NULL;
    TTree* rawEventTree=NULL;
    TFile* eventFile=NULL;
    TTree* eventTree=NULL;
    EventConverter* converter;
    RawEvent rawEvent;
    Event event;
    string dir = shmp->dir;
    string rawEventFileName = dir+rawEventFilePrefix+to_string(fileID)+".root";
    string eventFileName = dir+eventFilePrefix+to_string(fileID)+".root";
    if(shmp->kRawEventSave){
        // std::unique_lock<std::mutex> lock(mtx);
        rawEventFile = new TFile((rawEventFileName+".writing").c_str(), "RECREATE");
        rawEventFile->cd();
        rawEventTree = new TTree(rawEventTreeName.c_str(), rawEventTreeName.c_str());
        rawEventTree->Branch(rawEventBranchName.c_str(), &rawEvent);
        // cv.notify_one();
    }
    if(shmp->kEventSave){
        // std::unique_lock<std::mutex> lock(mtx);
        eventFile = new TFile((eventFileName+".writing").c_str(), "RECREATE");
        eventFile->cd();
        eventTree = new TTree(eventTreeName.c_str(), eventTreeName.c_str());
        eventTree->Branch(eventBranchName.c_str(), &event);
        converter = new EventConverter((dir+"eventParameters.json").c_str());
        // cv.notify_one();
    }
    for(int i =0 ;i<shmp->nEvents;i++){
        if(ptrArray[i*2]!=NULL && ptrArray[i*2+1]!=NULL){
            rawEventPtrArray[i]=ptrArray[i*2];
            rawEventPtrArray[i]->Add(ptrArray[i*2+1]);
            delete ptrArray[i*2+1];
        }else if(ptrArray[i*2]==NULL && ptrArray[i*2+1]!=NULL){
            rawEventPtrArray[i]=ptrArray[i*2+1];
        }
        else if(ptrArray[i*2]!=NULL && ptrArray[i*2+1]==NULL){
            rawEventPtrArray[i]=ptrArray[i*2];
        }else{
            cout<<"error, DataProcessor::rawEventProcess "<<endl;
        }  
    }
    for(int i=0;i<shmp->nEvents;i++){
        if(rawEventFile!=NULL){
            rawEvent = *(rawEventPtrArray[i]);
            rawEventTree->Fill();
        }
        if(eventFile!=NULL){
            event = *(converter->convert(*(rawEventPtrArray[i])));
            eventTree->Fill();
        }
        delete rawEventPtrArray[i];
    }
    // std::unique_lock<std::mutex> lock(mtx);
    if(rawEventFile!=NULL){
        rawEventFile->cd();
        rawEventFile->Write();
        rawEventFile->Close();
        delete rawEventFile;
        rename((rawEventFileName+".writing").c_str(),rawEventFileName.c_str());
    }
    if(eventFile!=NULL){
        eventFile->cd();
        eventFile->Write();
        eventFile->Close();
        delete eventFile;
        rename((eventFileName+".writing").c_str(),eventFileName.c_str());
        delete converter;
    }
    delete[] rawEventPtrArray;
    delete[] ptrArray;
    // cv.notify_one();
}
void DataProcessor::msgReceiver(){
    string host1 = shmp->dataHost1;
    int port1 = shmp->dataPort1;
    string host2 = shmp->dataHost2;
    int port2 = shmp->dataPort2;
    AutoSocket* autoSocket1= new AutoSocket(port1,host1.c_str());//用来接收从SiTCP发送来的数据
    AutoSocket* autoSocket2= new AutoSocket(port2,host2.c_str());//用来接收从SiTCP发送来的数据
    TMessage* msg1=NULL;
    TMessage* msg2=NULL;
    RawEvent* rawSubEvent1=NULL;
    RawEvent* rawSubEvent2=NULL;
    while(shmp->status == status_running){
        if(rawSubEventBufferDQ.size()>10){
            sleep(1);
            std::cout<<"too many DP bufferes(>10)"<<endl;
            continue;
        }
        RawEvent** ptrArray=new RawEvent*[shmp->nEvents*2];//指针数组，用来缓存接收到的数据
        for(int i=0;i<shmp->nEvents;i++){
            while(true){
                if(shmp->status != status_running){
                    // for(int j=0;j<i;j++){
                    //     delete ptrArray[j];
                    //     delete ptrArray[j+1]; 
                    // }
                    delete ptrArray;
                    delete autoSocket1;
                    delete autoSocket2;
                    return;
                }
                if(msg1==NULL) {
                    msg1 = autoSocket1->get();
                    if(msg1==NULL)continue;
                    if(msg1->What()!=kMESS_OBJECT){
                        std::cout<<"unkown message type: msg1->What() = "<<msg1->What()<<endl;
                        delete msg1;
                        msg1=NULL;
                        continue;
                    }
                }
                if(msg2==NULL) {
                    msg2 = autoSocket2->get();
                    if(msg2==NULL)continue;
                    if(msg2->What()!=kMESS_OBJECT){
                        std::cout<<"unkown message type: msg2->What() = "<<msg2->What()<<endl;
                        delete msg2;
                        msg2=NULL;
                        continue;
                    }
                }
                if(rawSubEvent1==NULL) {
                    rawSubEvent1 = (RawEvent*)msg1->ReadObject(msg1->GetClass());
                    cout<<"received rawEvent sub1 ID "<<rawSubEvent1->event_id<<" channels "<<rawSubEvent1->channels.size()<<endl;
                }
                if(rawSubEvent2==NULL) {
                    rawSubEvent2 = (RawEvent*)msg2->ReadObject(msg2->GetClass());
                    cout<<"received rawEvent sub2 ID "<<rawSubEvent2->event_id<<" channels "<<rawSubEvent2->channels.size()<<endl;
                }
                if(rawSubEvent1->event_id==rawSubEvent2->event_id){
                    ptrArray[i*2]=rawSubEvent1;
                    ptrArray[i*2+1]=rawSubEvent2;
                    delete msg1;
                    msg1=NULL;
                    delete msg2;
                    msg2=NULL;
                    rawSubEvent1=NULL;
                    rawSubEvent2=NULL;
                    shmp->currentEventID=ptrArray[i*2]->event_id;
                }else if(rawSubEvent1->event_id>rawSubEvent2->event_id){
                    ptrArray[i*2]=NULL;
                    ptrArray[i*2+1]=rawSubEvent2;
                    delete msg2;
                    msg2=NULL;
                    rawSubEvent2=NULL;
                    shmp->currentEventID=ptrArray[i*2+1]->event_id;
                }else{
                    ptrArray[i*2]=rawSubEvent1;
                    ptrArray[i*2+1]=NULL;
                    delete msg1;
                    msg1=NULL;
                    rawSubEvent1=NULL;
                    shmp->currentEventID=ptrArray[i*2]->event_id;
                }
                if(shmp->nMakePar>0){
                    RawEvent* revt;
                    if(ptrArray[i*2]!=NULL && ptrArray[i*2+1]!=NULL){
                        revt=ptrArray[i*2];
                        revt->Add(ptrArray[i*2+1]);
                        delete ptrArray[i*2+1];
                    }else if(ptrArray[i*2]==NULL){
                        revt = ptrArray[i*2+1];
                    }else{
                        revt = ptrArray[i*2];
                    }
                    makePar(revt);
                    delete revt;
                    ptrArray[i*2]=NULL;
                    ptrArray[i*2+1]=NULL;
                    continue;
                }
                shmp->totalEvent++;
                break;
            }
        }
        rawSubEventBufferDQ.push_back(ptrArray);//把指针数组存放在一个容器中，供后续的处理。
    }
    delete autoSocket1,autoSocket2;
}
void DataProcessor::makePar(RawEvent* revt){
    parameter.fill(revt);
    shmp->nMakePar--;
    string dir = shmp->dir;
    if(shmp->nMakePar == 0){
        ofstream file1((dir+"thresholdes.json").c_str());
        file1<<parameter.getThreshold();
        file1.close();
        ofstream file2((dir+"eventParameters.json").c_str());
        // file2<<parameter.getSettings(WValue,Vdrift);
        file2<<parameter.getSettings(shmp->WValue,shmp->Vdrift,shmp->FPC2,shmp->ElectronicFile,shmp->MicromegasFile);
        file2.close();
        cout<<"parameter generations done"<<endl;
    }
}
void DataProcessor::updateEventFileID(){
    eventFileID = 0;
    for (const auto & file : std::filesystem::directory_iterator(shmp->dir)){
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
    for (const auto & file : std::filesystem::directory_iterator(shmp->dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, rawEventFilePrefix.size()) != rawEventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(rawEventFilePrefix.size(),name.size() - suffix.size()));
        if(id>rawEventFileID)rawEventFileID = id;
    }
}
void DataProcessor::setFPC2(std::vector<std::map<string,int>> fpc2){
    // std::vector<std::map<string, int>>().swap(shmp->FPC2);
    json FPC2Json;
    for (const auto& map : fpc2) {
        json mapJson;
        // shmp->FPC2.push_back(map);
        // 输出map的键和值
        for (const auto& pair : map) {
            mapJson[pair.first] = pair.second;
            // cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
        }
        FPC2Json.push_back(mapJson);
    }
    string FPC2JsonString = FPC2Json.dump();
    strcpy(shmp->FPC2, FPC2JsonString.c_str());
}