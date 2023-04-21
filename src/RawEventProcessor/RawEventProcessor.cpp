#include "RawEventProcessor.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <errno.h>

using json = nlohmann::json;

ClassImp(RawEventProcessor);

RawEventProcessor::RawEventProcessor(){

   SEMFileID = sem_open("REP", O_RDWR|O_CREAT, 0666, 1);
    if(SEMFileID == SEM_FAILED){
        if(errno == EEXIST){
            SEMFileID = sem_open("REP",0);
            sem_init(SEMFileID,1,1);
        }else{
            cout<<"SEM error, RawEventProcessor::RawEventProcessor()"<<endl;
        }
    }
   rawEvent = new RawEvent();
}

RawEventProcessor::~RawEventProcessor(){
    sem_close(SEMFileID);
    sem_unlink("REP");
}
void RawEventProcessor::setRawTreeName(const char* name){rawTreeName = name;}
void RawEventProcessor::setRawBranchName(const char* name){rawBranchName = name;}
void RawEventProcessor::setEventTreeName(const char* name){eventTreeName = name;}
void RawEventProcessor::setEventBranchName(const char* name){eventBranchName = name;}
void RawEventProcessor::updateSettings(const char* msg){
  maker.updateSettings(msg);
}
void RawEventProcessor::setOutputFilePrefix(const char* prefix){
  outputFilePrefix = prefix;
}
void RawEventProcessor::setDir(const char* _dir){
  dir = _dir;
  if(dir[dir.length()-1]!='/')dir.append("/");
  std::filesystem::create_directory(dir.c_str());

}
void RawEventProcessor::setRawEventFilePrefix(const char* prefix){
  rawEventFilePrefix = prefix;
}

void RawEventProcessor::updateOutputFileID(){
    int outputFileMaxID = -1;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, outputFilePrefix.size()) != outputFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(outputFilePrefix.size(),name.size() - suffix.size()));
        if(id>outputFileMaxID)outputFileMaxID = id;
    }
    outputFileID = -1;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, rawEventFilePrefix.size()) != rawEventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(rawEventFilePrefix.size(),name.size() - suffix.size()));
        if(id<=outputFileMaxID)continue;
        if((id<outputFileID)||(outputFileID == -1))outputFileID = id;
    }

}

string RawEventProcessor::getEventFileList(int n){
    string list;
    int count = 0;
    for (const auto & file : std::filesystem::directory_iterator(dir)){
        if(strstr(file.path().c_str(),outputFilePrefix.c_str())==NULL)continue;
        list.append(file.path());
        list.append(";");
        count ++;
        if(n>0&&count>n)break;
    }
    return list;
}

void RawEventProcessor::openRawEventFile(){
    string filename = dir+rawEventFilePrefix+to_string(outputFileID)+".root";
    rawEventFile = new TFile(filename.c_str(), "READ"); 
    rawTree = (TTree*) rawEventFile->Get(rawTreeName);
    rawTree->SetBranchAddress(rawBranchName,&rawEvent);
}

void RawEventProcessor::createEventFile(){
    sem_wait(SEMFileID);
    updateOutputFileID();
    if(outputFileID>=0){
        openRawEventFile();
      string filename = dir+outputFilePrefix+to_string(outputFileID)+".root";
      eventFile = new TFile(filename.c_str(), "RECREATE");
      eventFile->cd();
      eventTree = new TTree(eventTreeName, eventTreeName);
      eventTree->Branch(eventBranchName, &event);
      
    }
    sem_post(SEMFileID);
}

void RawEventProcessor::closeRawEventFile(){
    rawEventFile->Close();
    delete rawEventFile;
}

void RawEventProcessor::closeEventFile(){
    eventFile->cd();
    eventFile->Write();
    eventFile->Close();
    delete eventFile;
}

void RawEventProcessor::stop(){
    status = status_stopping;
    mThread->join();
}
void RawEventProcessor::run(){
    mThread = new thread(&RawEventProcessor::mLoop, this);
}
// void RawEventProcessor::setSocketPort(int port){
//     socketPort = port;
// }
void RawEventProcessor::mLoop(){
    cout<<"raw event convertion to event loop start"<<endl;
    status = status_running;
    // TMessageSocket* socket = new TMessageSocket(socketPort);
    while(status == status_running){
        createEventFile();
        if(outputFileID<0){
          sleep(1);
          continue;
        }
        int n = rawTree->GetEntries();
        for( int i = 0; i<n;i++){
          rawTree->GetEntry(i);
          event = *maker.convert(*rawEvent);
          eventTree->Fill();
        //   socket->put(rawEvent);
        //   socket->put(&event);
        }
        closeRawEventFile();
        closeEventFile();
    }
    // delete socket;
    cout<<"raw event convertion to event loop stop"<<endl;
}
