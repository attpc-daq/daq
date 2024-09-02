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
    shmid = shmget(shmkey, sizeof(struct shmseg)+128, 0644|IPC_CREAT);
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
    shmp->nRawEventProcessor = 0;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    LockFreeQueue<RawEvent*> *subRawEvent1Queue = new LockFreeQueue<RawEvent*>();
    LockFreeQueue<RawEvent*> *subRawEvent2Queue = new LockFreeQueue<RawEvent*>();
    LockFreeQueue<LockFreeQueue<RawEvent*> *> *rawEventQueues = new LockFreeQueue<LockFreeQueue<RawEvent*> *>();
    LockFreeQueue<LockFreeQueue<RawEvent*> *> *rawEventQueues4QA = new LockFreeQueue<LockFreeQueue<RawEvent*> *>();
    // LockFreeQueue<LockFreeQueue<Event*> *> *eventQueues4QA = new LockFreeQueue<LockFreeQueue<Event*> *>();

    thread* mSubEvent1Receiver = new thread(&DataProcessor::subRawEvent1Receiver, this, subRawEvent1Queue);
    thread* mSubEvent2Receiver = new thread(&DataProcessor::subRawEvent2Receiver, this, subRawEvent2Queue);
    thread* mRawEventCombinator = new thread(&DataProcessor::rawEventCombinator, this, subRawEvent1Queue, subRawEvent2Queue, rawEventQueues);
    thread* mRawEventProcess = new thread(&DataProcessor::rawEventProcess, this, rawEventQueues, rawEventQueues4QA);
    thread* mDataSenderLoop = new thread(&DataProcessor::DataSenderLoop, this, rawEventQueues4QA);

    mSubEvent1Receiver->join();
    mSubEvent2Receiver->join();
    mRawEventCombinator->join();
    mRawEventProcess->join();
    mDataSenderLoop->join();
    shmp->status = status_stopped;
    delete subRawEvent1Queue, subRawEvent2Queue, rawEventQueues;
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
int DataProcessor::getNRawEventProcessor(){
    return shmp->nRawEventProcessor;
}
void DataProcessor::setDebug(bool debug){
    shmp->kDebug = debug;
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

void DataProcessor::setDataPort4QA(int port){
    shmp->dataPort4QA=port;
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
void DataProcessor::subRawEvent1Receiver(LockFreeQueue<RawEvent*> *queue){
    string host = shmp->dataHost1;
    int port = shmp->dataPort1;
    AutoSocket* autoSocket= new AutoSocket(port,host.c_str());
    RawEvent * obj = NULL;
    RawEvent * objNext = NULL;
    while(shmp->status == status_running){
        if(queue->getSize()>100){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        if(obj == NULL) obj = (RawEvent *) autoSocket->get(RawEvent::Class());
        if(obj == NULL){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        if(objNext == NULL) objNext = (RawEvent *) autoSocket->get(RawEvent::Class());
        if(objNext == NULL){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        // cout<<"sub1:"<<obj->event_id<<" "<<objNext->event_id<<endl;
        if(obj->event_id < objNext->event_id){
            queue->push(obj);
            obj = objNext;
            objNext = NULL;
        // }else if(obj->event_id == objNext->event_id){
        //     objNext->Add(obj);
        //     cout<<"event1 adding:"<<objNext->event_id<<"channels:"<<size(objNext->channels)<<"+"<<size(obj->channels)<<endl;
        //     delete obj;
        }else{
            // cout<<"Warning: ignore wrong rawEvent(sub1) id:"<<obj->event_id
            // <<" ch:"<<size(obj->channels)
            // <<" Next:"<<objNext->event_id
            // <<" ch:"<<size(objNext->channels)
            // <<endl;
            delete objNext;
            objNext = NULL;
        }
        // obj = objNext;
        // objNext = NULL;
    }
    if(obj != NULL)delete obj;
    if(objNext != NULL) delete objNext;
}
void DataProcessor::subRawEvent2Receiver(LockFreeQueue<RawEvent*> *queue){
    string host = shmp->dataHost2;
    int port = shmp->dataPort2;
    AutoSocket* autoSocket= new AutoSocket(port,host.c_str());
    RawEvent * obj = NULL;
    RawEvent * objNext = NULL;
    while(shmp->status == status_running){
        if(queue->getSize()>100){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        if(obj == NULL) obj = (RawEvent *) autoSocket->get(RawEvent::Class());
        if(obj == NULL){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        if(objNext == NULL) objNext = (RawEvent *) autoSocket->get(RawEvent::Class());
        if(objNext == NULL){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        // cout<<"sub2:"<<obj->event_id<<" "<<objNext->event_id<<endl;
        if(obj->event_id < objNext->event_id){
            queue->push(obj);
            obj = objNext;
            objNext = NULL;
        // }else if(obj->event_id == objNext->event_id){
        //     objNext->Add(obj);
        //     cout<<"event2 adding:"<<objNext->event_id<<"channels:"<<size(objNext->channels)<<"+"<<size(obj->channels)<<endl;
        //     delete obj;
        }else{
            // cout<<"Warning: ignore wrong rawEvent(sub2) id:"<<obj->event_id
            // <<" ch:"<<size(obj->channels)
            // <<" Next:"<<objNext->event_id
            // <<" ch:"<<size(objNext->channels)
            // <<endl;
            delete objNext;
            objNext = NULL;
        }
        // obj = objNext;
        // objNext = NULL;
    }
    if(obj != NULL)delete obj;
    if(objNext != NULL) delete objNext;
}
void DataProcessor::rawEventCombinator(LockFreeQueue<RawEvent*> *subRawEvent1Queue, 
                                       LockFreeQueue<RawEvent*> *subRawEvent2Queue,
                                       LockFreeQueue<LockFreeQueue<RawEvent*> *> *rawEventQueues){
    RawEvent* subRawEvent1=NULL;
    RawEvent* subRawEvent2=NULL;
    RawEvent* rawEvent=NULL;
    size_t eventCount = 0;
    size_t queueCount = 0;
    LockFreeQueue<RawEvent*> * rawEventQueue;
    while(shmp->status == status_running){
        if(subRawEvent1 == NULL){
            if(!(subRawEvent1Queue->pop(subRawEvent1))) subRawEvent1=NULL;
        }
        if(subRawEvent2 == NULL){
            if(!(subRawEvent2Queue->pop(subRawEvent2))) subRawEvent2=NULL;
        }
        if(subRawEvent1 == NULL || subRawEvent2 == NULL ) continue;
        // cout<<"sub1:"<<subRawEvent1->event_id<<" ch:"<<size(subRawEvent1->channels)<<" ptr "<<subRawEvent1<<" sub2:"<<subRawEvent2->event_id<<" ch:"<<size(subRawEvent2->channels)<<" ptr "<<subRawEvent2<<endl;
        if(subRawEvent1->event_id==subRawEvent2->event_id){
            rawEvent = subRawEvent1;
            rawEvent->Add(subRawEvent2);
            delete subRawEvent2;
            shmp->currentEventID = rawEvent->event_id;
            subRawEvent1 = NULL;
            subRawEvent2 = NULL;
        }else if(subRawEvent1->event_id>subRawEvent2->event_id){
            rawEvent = subRawEvent2;
            shmp->currentEventID = rawEvent->event_id;
            subRawEvent2 = NULL;
        }else{
            rawEvent = subRawEvent1;
            shmp->currentEventID = rawEvent->event_id;
            subRawEvent1 = NULL;
        }
        shmp->totalEvent++;
        // cout<<"--->comb event:"<<rawEvent->event_id<<" ch:"<<size(rawEvent->channels)<<" ptr "<<rawEvent<<endl;
        //
        if(shmp->nMakePar>0){
            makePar(rawEvent);
            delete rawEvent;
            continue;
        }
        //
        if(shmp->kDebug) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(eventCount == 0){
            rawEventQueue = new  LockFreeQueue<RawEvent*>();
            while(true){
                if(rawEventQueues->push(rawEventQueue, queueCount))break;
                if(shmp->status > status_running) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            queueCount++;
        }
        rawEventQueue->push(rawEvent);
        eventCount++;
        if(eventCount>=shmp->nEvents){
            eventCount = 0;
            rawEventQueue->stop();
        }
    }
}
void DataProcessor::rawEventProcess(LockFreeQueue<LockFreeQueue<RawEvent*> *> *rawEventQueues, 
                                    LockFreeQueue<LockFreeQueue<RawEvent*> *> *rawEventQueues4QA){
    updateRawEventFileID();
    updateEventFileID();
    shmp->outputFileID = eventFileID > rawEventFileID ? eventFileID : rawEventFileID;
    LockFreeQueue<RawEvent*> *rawEventQueue;
    int rawEventQueue4QAID = 0;
    while(shmp->status == status_running){
        if(rawEventQueues->pop(rawEventQueue)){
            shmp->outputFileID++;
            int fileID = shmp->outputFileID;
            bool isQA = true;
            if(rawEventQueues4QA->getSize()>0) isQA = false;
            LockFreeQueue<RawEvent*> *rawEventQueue4QA = NULL;
            if(isQA){
                rawEventQueue4QA = new LockFreeQueue<RawEvent*>();
                while(true){
                    if(rawEventQueues4QA->push(rawEventQueue4QA, rawEventQueue4QAID))break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    if(shmp->status >= status_running)break;
                }
                rawEventQueue4QAID++;
            }
            shmp->nRawEventProcessor++;
            new thread(&DataProcessor::rawEventProcessor, this, rawEventQueue,fileID, rawEventQueue4QA);
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    while(shmp->nRawEventProcessor>0){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void DataProcessor::rawEventProcessor(LockFreeQueue<RawEvent*> *rawEventQueue,
                                      int fileID,
                                      LockFreeQueue<RawEvent*> *rawEventQueue4QA){
    RawEvent * rawEventPtr;
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
        rawEventFile = new TFile((rawEventFileName+".writing").c_str(), "RECREATE");
        rawEventFile->cd();
        rawEventTree = new TTree(rawEventTreeName.c_str(), rawEventTreeName.c_str());
        rawEventTree->Branch(rawEventBranchName.c_str(), &rawEvent);
    }
    if(shmp->kEventSave){
        eventFile = new TFile((eventFileName+".writing").c_str(), "RECREATE");
        eventFile->cd();
        eventTree = new TTree(eventTreeName.c_str(), eventTreeName.c_str());
        eventTree->Branch(eventBranchName.c_str(), &event);
        converter = new EventConverter((dir+"eventParameters.json").c_str());
    }
    while(shmp->status == status_running){
        if(!(rawEventQueue->pop(rawEventPtr))){
            if(rawEventQueue->isStopped())break;
            continue;
        }
        if(rawEventFile!=NULL){
            rawEvent = *rawEventPtr;
            rawEventTree->Fill();
        }
        if(eventFile!=NULL){
            event = *(converter->convert(*rawEventPtr));
            eventTree->Fill();
        }
        if(rawEventQueue4QA != NULL){
            rawEventQueue4QA->push(rawEventPtr);
        }else{
            delete rawEventPtr;
        }
    }
    if(rawEventQueue4QA != NULL) rawEventQueue4QA->stop();
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
    shmp->nRawEventProcessor--;
}
void DataProcessor::DataSenderLoop(LockFreeQueue<LockFreeQueue<RawEvent*> *> *rawEventQueues4QA){
    AutoSocket* autoSocket4QA = new AutoSocket(shmp->dataPort4QA);
    LockFreeQueue<RawEvent*> *rawEventQueue4QA;
    RawEvent* rawEvent;
    while(shmp->status == status_running){
        if(rawEventQueues4QA->pop(rawEventQueue4QA)){
            while(shmp->status == status_running){
                if(!(rawEventQueue4QA->pop(rawEvent))){
                    if(rawEventQueue4QA->isStopped())break;
                    continue;
                }
                autoSocket4QA->send(rawEvent);
                delete rawEvent;
            }
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
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
