#include "DAQHandler.h"


DAQHandler::DAQHandler(){
    // SiTCP
    commands["getSiTCPState"] = {std::bind(&DAQHandler::getSiTCPState, this, std::placeholders::_1),""};
    commands["initSitcp"] = {std::bind(&DAQHandler::initSitcp, this, std::placeholders::_1),""};
    commands["shutdownSiTCP"] = {std::bind(&DAQHandler::shutdownSiTCP, this, std::placeholders::_1),""};
    commands["startSiTCP"] = {std::bind(&DAQHandler::startSiTCP, this, std::placeholders::_1),""};
    commands["stopSiTCP"] = {std::bind(&DAQHandler::stopSiTCP, this, std::placeholders::_1),""};
    commands["startSiTCPData"] = {std::bind(&DAQHandler::startSiTCPData, this, std::placeholders::_1),""};
    commands["stopSiTCPData"] = {std::bind(&DAQHandler::stopSiTCPData, this, std::placeholders::_1),""};
    commands["startSiTCPDecoder"] = {std::bind(&DAQHandler::startSiTCPDecoder, this, std::placeholders::_1),""};
    commands["stopSiTCPDecoder"] = {std::bind(&DAQHandler::stopSiTCPDecoder, this, std::placeholders::_1),""};
    commands["getSiTCP1DataRate"] = {std::bind(&DAQHandler::getSiTCP1DataRate, this, std::placeholders::_1),""};
    commands["getSiTCP2DataRate"] = {std::bind(&DAQHandler::getSiTCP2DataRate, this, std::placeholders::_1),""};
    commands["getSiTCP1NTask"] = {std::bind(&DAQHandler::getSiTCP1NTask, this, std::placeholders::_1),""};
    commands["getSiTCP2NTask"] = {std::bind(&DAQHandler::getSiTCP2NTask, this, std::placeholders::_1),""};
    commands["getSiTCPDataAcquisitionState"] = {std::bind(&DAQHandler::getSiTCPDataAcquisitionState, this, std::placeholders::_1),""};
    commands["getSiTCPDecoderState"] = {std::bind(&DAQHandler::getSiTCPDecoderState, this, std::placeholders::_1),""};
    commands["sendToSiTCP1"] = {std::bind(&DAQHandler::sendToSiTCP1, this, std::placeholders::_1),""};
    commands["sendToSiTCP2"] = {std::bind(&DAQHandler::sendToSiTCP2, this, std::placeholders::_1),""};
    commands["setBufferFileSize"] = {std::bind(&DAQHandler::setBufferFileSize, this, std::placeholders::_1),""};

    // DataProcessor
    commands["getDataProcessorState"] = {std::bind(&DAQHandler::getDataProcessorState, this, std::placeholders::_1),""};
    commands["initDataProcessor"] = {std::bind(&DAQHandler::initDataProcessor, this, std::placeholders::_1),""};
    commands["shutdownDataProcessor"] = {std::bind(&DAQHandler::shutdownDataProcessor, this, std::placeholders::_1),""};
    commands["startDataProcessor"] = {std::bind(&DAQHandler::startDataProcessor, this, std::placeholders::_1),""};
    commands["stopDataProcessor"] = {std::bind(&DAQHandler::stopDataProcessor, this, std::placeholders::_1),""};
    commands["getDataProcessorTotalEvent"] = {std::bind(&DAQHandler::getDataProcessorTotalEvent, this, std::placeholders::_1),""};
    commands["getDataProcessorCurrentEventID"] = {std::bind(&DAQHandler::getDataProcessorCurrentEventID, this, std::placeholders::_1),""};
    commands["getDataProcessorCurrentFileID"] = {std::bind(&DAQHandler::getDataProcessorCurrentFileID, this, std::placeholders::_1),""};
    commands["setDataProcessorParameterEvents"] = {std::bind(&DAQHandler::setDataProcessorParameterEvents, this, std::placeholders::_1),""};
    commands["getDataProcessorParameterEvents"] = {std::bind(&DAQHandler::getDataProcessorParameterEvents, this, std::placeholders::_1),""};
    commands["turnOnRawEventSave"] = {std::bind(&DAQHandler::turnOnRawEventSave, this, std::placeholders::_1),""};
    commands["turnOffRawEventSave"] = {std::bind(&DAQHandler::turnOffRawEventSave, this, std::placeholders::_1),""};
    commands["turnOnEventSave"] = {std::bind(&DAQHandler::turnOnEventSave, this, std::placeholders::_1),""};
    commands["turnOffEventSave"] = {std::bind(&DAQHandler::turnOffEventSave, this, std::placeholders::_1),""};

    // QA
    commands["initQA"] = {std::bind(&DAQHandler::initQA, this, std::placeholders::_1),""};
    commands["shutdownQA"] = {std::bind(&DAQHandler::shutdownQA, this, std::placeholders::_1),""};
    commands["startQA"] = {std::bind(&DAQHandler::startQA, this, std::placeholders::_1),""};
    commands["stopQA"] = {std::bind(&DAQHandler::stopQA, this, std::placeholders::_1),""};
    commands["getQAState"] = {std::bind(&DAQHandler::getQAState, this, std::placeholders::_1),""};
    commands["getQATotalEvent"] = {std::bind(&DAQHandler::getQATotalEvent, this, std::placeholders::_1),""};
    commands["getQACurrentEventID"] = {std::bind(&DAQHandler::getQACurrentEventID, this, std::placeholders::_1),""};
    
    sitcp[0]=NULL;
    sitcp[1]=NULL;
    dataProcessor = NULL;
}
DAQHandler::~DAQHandler(){}

std::string DAQHandler::response(const char* request){
    // std::cout<<"Request: "<<request<<std::endl;
    std::stringstream iss(request);
    std::string func;
    iss >> func;
    string rsp = "Command not found";
    auto it = commands.find(func);
    if (it != commands.end()){
        std::string args;
        std::string arg;
        while (iss >> arg){
            args += arg + " ";
        }
        // return it->second.function(args.c_str());
        rsp = it->second.function(args.c_str());
    }
    return rsp;
}
std::string DAQHandler::getSiTCPState(const char* args){
    string rsp = "SiTCPState ";
    if(sitcp[0]==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(sitcp[0]->getState())+" ");
    if(sitcp[1]==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(sitcp[1]->getState()));
    return rsp;
}
std::string DAQHandler::getSiTCPDataAcquisitionState(const char* args){
    string rsp = "SiTCPDataAcquisitionState ";
    if(sitcp[0]==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(sitcp[0]->getDataAcquisitionState())+" ");
    if(sitcp[1]==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(sitcp[1]->getDataAcquisitionState()));
    return rsp;
}
std::string DAQHandler::getSiTCPDecoderState(const char* args){
    string rsp = "SiTCPDecoderState ";
    if(sitcp[0]==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(sitcp[0]->getDecoderState())+" ");
    if(sitcp[1]==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(sitcp[1]->getDecoderState()));
    return rsp;
}
std::string DAQHandler::initSitcp(const char* args){
    stringstream iss(args);
    string ip1,ip2;
    int port1,port2,fileMaxSize;
    string dir1,dir2;
    iss>>ip1>>port1>>dir1>>ip2>>port2>>dir2>>fileMaxSize;
    cout<<args<<endl;
    sitcp[0] = new SiTCP(1);
    sitcp[0]->resetSHM();
    sitcp[0]->setServerAddress(ip1.c_str(),port1);
    sitcp[0]->setDir(dir1.c_str());
    sitcp[0]->setDataPort(8010);
    sitcp[0]->setFileMaxSize(fileMaxSize);
    sitcp[0]->disableBuffer();


    sitcp[1] = new SiTCP(2);
    sitcp[1]->resetSHM();
    sitcp[1]->setServerAddress(ip2.c_str(),port2);
    sitcp[1]->setDir(dir2.c_str());
    sitcp[1]->setDataPort(8011);
    sitcp[1]->setFileMaxSize(fileMaxSize);
    sitcp[1]->disableBuffer();

    string rsp = "initSitcp Done";
    return rsp;
}
std::string DAQHandler::shutdownSiTCP(const char* args){
    if(sitcp[0]!=NULL){
        delete sitcp[0];
        sitcp[0] = NULL;
    }
    if(sitcp[1]!=NULL){
        delete sitcp[1];
        sitcp[1] = NULL;
    }
    string rsp = "shutdownSiTCP Done";
    return rsp;
}
string DAQHandler::startSiTCP(const char* args){
    sitcp[0]->start();
    sitcp[1]->start();
    string rsp = "startSiTCP Done";
    return rsp;
}
string DAQHandler::stopSiTCP(const char* args){
    sitcp[0]->stop();
    sitcp[1]->stop();
    string rsp = "stopSiTCP Done";
    return rsp;
}
string DAQHandler::startSiTCPData(const char* args){
    sitcp[0]->startData();
    sitcp[1]->startData();
    string rsp = "startSiTCPData Done";
    return rsp;
}
string DAQHandler::stopSiTCPData(const char* args){
    sitcp[0]->stopData();
    sitcp[1]->stopData();
    string rsp = "stopSiTCPData Done";
    return rsp;
}
string DAQHandler::startSiTCPDecoder(const char* args){
    sitcp[0]->startDecoder();
    sitcp[1]->startDecoder();
    string rsp = "startSiTCPDecoder Done";
    return rsp;
}
string DAQHandler::stopSiTCPDecoder(const char* args){
    sitcp[0]->stopDecoder();
    sitcp[1]->stopDecoder();
    string rsp = "stopSiTCPDecoder Done";
    return rsp;
}
string DAQHandler::getSiTCP1DataRate(const char* args){
    string rsp="dataRate1 ";
    if(sitcp[0]==NULL)rsp.append("-1");
    else rsp.append(std::to_string(sitcp[0]->getRate()));
    return rsp;
}
string DAQHandler::getSiTCP2DataRate(const char* args){
    string rsp="dataRate2 ";
    if(sitcp[1]==NULL)rsp.append("-1");
    else rsp.append(std::to_string(sitcp[1]->getRate()));
    return rsp;
}
string DAQHandler::getSiTCP1NTask(const char* args){
    string rsp="nTask1 ";
    if(sitcp[0]==NULL)rsp.append("-1");
    else rsp.append(std::to_string(sitcp[0]->getNTasks()));
    return rsp;
}
string DAQHandler::getSiTCP2NTask(const char* args){
    string rsp="nTask2 ";
    if(sitcp[1]==NULL)rsp.append("-1");
    else rsp.append(std::to_string(sitcp[1]->getNTasks()));
    return rsp;
}
string DAQHandler::setBufferFileSize(const char* args){
    stringstream iss(args);
    int bufferSize;
    iss>>bufferSize;
    sitcp[0]->setFileMaxSize(bufferSize);
    sitcp[1]->setFileMaxSize(bufferSize);
    string rsp = "setBufferFileSize Done";
    return rsp;
}

string DAQHandler::sendToSiTCP1(const char* args){
    // stringstream iss(args);
    // int id;
    // string cmd;
    // iss>>id>>cmd;
    // if(id==1) sitcp[0]->sendCommand(cmd.c_str());
    // else if(id==2) sitcp[1]->sendCommand(cmd.c_str());
    sitcp[0]->sendToSiTCP(args);
    string rsp = "sendToSiTCP Done";
    return rsp;
}
string DAQHandler::sendToSiTCP2(const char* args){
    // stringstream iss(args);
    // int id;
    // string cmd;
    // iss>>id>>cmd;
    // if(id==1) sitcp[0]->sendCommand(cmd.c_str());
    // else if(id==2) sitcp[1]->sendCommand(cmd.c_str());
    string rsp = "sendToSiTCP Done";
    return rsp;
}

std::string DAQHandler::getDataProcessorState(const char* args){
    string rsp = "DataProcessorState ";
    if(dataProcessor==NULL) rsp.append("-1 ");
    else rsp.append(std::to_string(dataProcessor->getState()));
    return rsp;
}
std::string DAQHandler::initDataProcessor(const char* args){
    stringstream iss(args);
    string dir;
    int fileEvents;
    iss>>dir>>fileEvents;
    dataProcessor = new DataProcessor();
    dataProcessor->resetSHM();
    dataProcessor->setDir(dir.c_str());
    dataProcessor->setDataPort(8010,"localhost",8011,"localhost");
    dataProcessor->setEventSave(false);
    dataProcessor->setRawEventSave(false);
    dataProcessor->setFileEvents(fileEvents);
    string rsp = "initDataProcessor Done";
    return rsp;
}
std::string DAQHandler::shutdownDataProcessor(const char* args){
    if(dataProcessor!=NULL){
        delete dataProcessor;
        dataProcessor = NULL;
    }
    string rsp = "shutdownDataProcessor Done";
    return rsp;
}
std::string DAQHandler::startDataProcessor(const char* args){
    dataProcessor->start();
    string rsp = "startDataProcessor Done";
    return rsp;
}
std::string DAQHandler::stopDataProcessor(const char* args){
    dataProcessor->stop();
    string rsp = "stopDataProcessor Done";
    return rsp;
}
std::string DAQHandler::getDataProcessorTotalEvent(const char* args){
    string rsp = "DataProcessorTotalEvent ";
    if(dataProcessor==NULL) rsp.append("-1");
    else rsp.append(std::to_string(dataProcessor->getTotalEvent()));
    return rsp;
}
std::string DAQHandler::getDataProcessorCurrentEventID(const char* args){
    string rsp = "DataProcessorCurrentEventID ";
    if(dataProcessor==NULL) rsp.append("-1");
    else rsp.append(std::to_string(dataProcessor->getCurrentEventID()));
    return rsp;
}
std::string DAQHandler::getDataProcessorCurrentFileID(const char* args){
    string rsp = "DataProcessorCurrentFileID ";
    if(dataProcessor==NULL) rsp.append("-1");
    else rsp.append(std::to_string(dataProcessor->getOutputFileID()));
    return rsp;
}
std::string DAQHandler::setDataProcessorParameterEvents(const char* args){
    stringstream iss(args);
    int events;
    iss>>events;
    dataProcessor->generateParameters(events);
    string rsp = "setDataProcessorParameterEvents Done";
    return rsp;
}
std::string DAQHandler::getDataProcessorParameterEvents(const char* args){
    string rsp = "DataProcessorParameterEvents ";
    if(dataProcessor==NULL) rsp.append("-1");
    else rsp.append(std::to_string(dataProcessor->getNMakePar()));
    return rsp;
}
std::string DAQHandler::turnOnRawEventSave(const char* args){
    dataProcessor->setRawEventSave(true);
    string rsp = "turnOnRawEventSave Done";
    return rsp;
}
std::string DAQHandler::turnOffRawEventSave(const char* args){
    dataProcessor->setRawEventSave(false);
    string rsp = "turnOffRawEventSave Done";
    return rsp;
}
std::string DAQHandler::turnOnEventSave(const char* args){
    dataProcessor->setEventSave(true);
    string rsp = "turnOnEventSave Done";
    return rsp;
}
std::string DAQHandler::turnOffEventSave(const char* args){
    dataProcessor->setEventSave(false);
    string rsp = "turnOffEventSave Done";
    return rsp;
}

std::string DAQHandler::getQAState(const char* args){
    string rsp = "QAState ";
    if(eventQA==NULL) rsp.append("-1");
    else rsp.append(std::to_string(eventQA->getState()));
    return rsp;
}
std::string DAQHandler::initQA(const char* args){
    stringstream iss(args);
    int THttpServerPort;
    string dir;
    iss>>THttpServerPort>>dir;
    eventQA = new EventQA();
    eventQA->setTHttpServerPort(THttpServerPort);
    eventQA->setDir(dir.c_str());
    // ifstream file;
    // file.open("./output/eventParameters.json", ios::in);
    // string settingJson;
    // file>>settingJson;
    // eventQA->updateSettings(settingJson);
    // file.open(os.path.join(self.outputDir,'./eventParameters.json'), 'r')
    // self.eventQA.updateSettings(settingJson)
    string rsp = "initQA Done";
    return rsp;
}
std::string DAQHandler::shutdownQA(const char* args){
    if(eventQA!=NULL){
        delete eventQA;
        eventQA = NULL;
    }
    string rsp = "shutdownQA Done";
    return rsp;
}
std::string DAQHandler::startQA(const char* args){
    eventQA->start();
    string rsp = "startQA Done";
    return rsp;
}
std::string DAQHandler::stopQA(const char* args){
    eventQA->stop();
    string rsp = "stopQA Done";
    return rsp;
}

std::string DAQHandler::getQATotalEvent(const char* args){
    string rsp = "QATotalEvent ";
    if(eventQA==NULL) rsp.append("-1");
    else rsp.append(std::to_string(eventQA->getTotalEvent()));
    return rsp;
}
std::string DAQHandler::getQACurrentEventID(const char* args){
    string rsp = "QACurrentEventID ";
    if(eventQA==NULL) rsp.append("-1");
    else rsp.append(std::to_string(eventQA->getCurrentEventID()));
    return rsp;
}


