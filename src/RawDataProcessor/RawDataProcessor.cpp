#include "RawDataProcessor.h"

ClassImp(RawDataProcessor);

RawDataProcessor::RawDataProcessor(){
    nEvents = 100;
    writingTempFileName = "writing";
    readingTempFileName = "reading";
}
RawDataProcessor::~RawDataProcessor(){
}
void RawDataProcessor::setRawEventTreeName(const char* name){
    rawEventTreeName = name;
}
void RawDataProcessor::setRawEventBranchName(const char* name){
    rawEventBranchName = name;
}

void RawDataProcessor::setRawDataDir(const char* dir){
    rawDataDir = dir;
    if(dir[rawDataDir.length()-1]!='/')rawDataDir.append("/");
    std::filesystem::create_directory(rawDataDir.c_str());
}
void RawDataProcessor::setOutputDir(const char* dir){
    outputDir = dir;
    if(dir[outputDir.length()-1]!='/')outputDir.append("/");
    std::filesystem::create_directory(outputDir.c_str());
}
void RawDataProcessor::setOutputFileEvents(int n){
    nEvents = n;
}
void RawDataProcessor::setRawDataFilePrefix(const char* rawfileprefix){
    rawDataFilePrefix = rawfileprefix;
}
void RawDataProcessor::setOutputFilePrefix(const char* prefix){
    rawEventFilePrefix = prefix;
}
    
void RawDataProcessor::setReadingTempFileName(const char* name){
    readingTempFileName = name;
}
void RawDataProcessor::setWritingTempFileName(const char* name){
    writingTempFileName = name;
}
bool RawDataProcessor::openRawDataFile(){
    updateRawDataFileID();
    if(rawDataFileID<0)return false;
    int check = rename (( rawDataDir+rawDataFilePrefix+to_string(rawDataFileID)+".dat").c_str(), (rawDataDir+readingTempFileName).c_str() );
    if(check == -1) return false;
    rawDataFile.open((rawDataDir+readingTempFileName).c_str(), std::ios::binary);
    if (!rawDataFile.is_open()) {
        return false;
    }
    return true;
}
void RawDataProcessor::closeRawDataFile(){
    rawDataFile.close();
    remove((rawDataDir+readingTempFileName).c_str());
}
void RawDataProcessor::updateOutputFileID(){
    outputFileID = -1;
    for (const auto & file : std::filesystem::directory_iterator(outputDir)){
        string name = file.path().filename().string();
        string suffix = ".root";
        if(name.substr(0, rawEventFilePrefix.size()) != rawEventFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(rawEventFilePrefix.size(),name.size() - suffix.size()));
        if(id>outputFileID)outputFileID = id;
    }
    outputFileID++;
}
void RawDataProcessor::updateRawDataFileID(){
    rawDataFileID = -1;
    for (const auto & file : std::filesystem::directory_iterator(rawDataDir)){
        string name = file.path().filename().string();
        string suffix = ".dat";
        if(name.substr(0, rawDataFilePrefix.size()) != rawDataFilePrefix)continue;
        if(name.substr(name.size() - suffix.size()) != suffix)continue;
        int id = stoi(name.substr(rawDataFilePrefix.size(),name.size() - suffix.size()));
        if((id<rawDataFileID)||(rawDataFileID == -1))rawDataFileID = id;
    }
}
void RawDataProcessor::createOutputFile(){
    updateOutputFileID();
    rawEventFile = new TFile((outputDir+writingTempFileName).c_str(), "RECREATE");
    rawEventFile->cd();
    rawTree = new TTree(rawEventTreeName.c_str(), rawEventTreeName.c_str());
    rawTree->Branch(rawEventBranchName.c_str(), &rawEvent);
}
void RawDataProcessor::closeOutputFile(){
    rawEventFile->cd();
    rawEventFile->Write();
    rawEventFile->Close();
    delete rawEventFile;
    updateOutputFileID();
    rename ( (outputDir+writingTempFileName).c_str(), ( outputDir+rawEventFilePrefix+to_string(outputFileID)+".root").c_str() );
}

string RawDataProcessor::getRawEventFileList(int n){
    string list;
    int count = 0;
    for (const auto & file : std::filesystem::directory_iterator(outputDir)){
        if(strstr(file.path().c_str(),rawEventFilePrefix.c_str())==NULL)continue;
        list.append(file.path());
        list.append(";");
        count ++;
        if(n>0&&count>n)break;
    }
    return list;
}


string RawDataProcessor::getRawDataFileList(int n){
    string list;
    int count = 0;
    for (const auto & file : std::filesystem::directory_iterator(rawDataDir)){
        if(strstr(file.path().c_str(),rawDataFilePrefix.c_str())==NULL)continue;
        list.append(file.path());
        list.append(";");
        count ++;
        if(n>0&&count>n)break;
    }
    return list;
}

void RawDataProcessor::stop(){
    status = status_stopping;
    mThread->join();
}
void RawDataProcessor::run(bool broadcast){
    mThread = new thread(&RawDataProcessor::mLoop, this, broadcast);
}

void RawDataProcessor::setSocketPort(int port){
    socketPort = port;
}

void RawDataProcessor::mLoop(bool broadcast){
    cout<<"raw data processor loop start, broadcast 0 for no, 1 for yes. "<< broadcast <<endl;
    status = status_running;
    char byte;
    int eventCount = 0;
    createOutputFile();
    TMessageSocket* socket;
    if(broadcast) socket = new TMessageSocket(socketPort);
    while(status == status_running){
        if(!openRawDataFile()){
            sleep(1);
            continue;
        }
        while (rawDataFile.read(&byte, 1) && (status == status_running)) {
            if(decoder.Fill(&byte)){
                rawEvent = decoder.rawEvent;
                rawTree->Fill();
                if(broadcast) socket->put(&rawEvent);
                eventCount++;
                if(eventCount>nEvents){
                    closeOutputFile();
                    eventCount = 0;
                    createOutputFile();
                }
            }
        }
        closeRawDataFile();
    }
    closeRawDataFile();
    closeOutputFile();
    if(broadcast) delete socket;
    cout<<"raw data processor loop stop"<<endl;
}
