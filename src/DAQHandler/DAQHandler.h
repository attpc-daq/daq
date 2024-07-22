#ifndef __DAQHandler_h__
#define __DAQHandler_h__
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "SiTCP.h"
#include "DataProcessor.h"
#include "EventQA.h"

using namespace std;

struct Command {
    std::function<std::string (const char*)> function;
    std::string usage_text;
};

class DAQHandler{
    public:
        DAQHandler();
        virtual ~DAQHandler();
        std::string response(const char* request);
    private:
        
        std::unordered_map<std::string, Command> commands;
        std::string seps_ = " ";
        std::string clientKey = "example_key";

        SiTCP* sitcp[2];
        DataProcessor* dataProcessor;
        EventQA* eventQA;
        std::string getSiTCPState(const char* args);
        std::string initSitcp(const char* args);
        std::string shutdownSiTCP(const char* args);
        std::string startSiTCP(const char* args);
        std::string stopSiTCP(const char* args);
        std::string startSiTCPData(const char* args);
        std::string stopSiTCPData(const char* args);
        std::string startSiTCPDecoder(const char* args);
        std::string stopSiTCPDecoder(const char* args);
        std::string getSiTCP1DataRate(const char* args);
        std::string getSiTCP2DataRate(const char* args);
        std::string getSiTCP1NTask(const char* args);
        std::string getSiTCP2NTask(const char* args);
        std::string getSiTCPDataAcquisitionState(const char* args);
        std::string getSiTCPDecoderState(const char* args);
        std::string sendToSiTCP1(const char* args);
        std::string sendToSiTCP2(const char* args);
        std::string setBufferFileSize(const char* args);

        std::string getDataProcessorState(const char* args);
        std::string initDataProcessor(const char* args);
        std::string shutdownDataProcessor(const char* args);
        std::string startDataProcessor(const char* args);
        std::string stopDataProcessor(const char* args);
        
        std::string getDataProcessorTotalEvent(const char* args);
        std::string getDataProcessorCurrentEventID(const char* args);
        std::string getDataProcessorCurrentFileID(const char* args);
        std::string setDataProcessorParameterEvents(const char* args);
        std::string getDataProcessorParameterEvents(const char* args);

        std::string turnOnRawEventSave(const char* args);
        std::string turnOffRawEventSave(const char* args);
        std::string turnOnEventSave(const char* args);
        std::string turnOffEventSave(const char* args);

        std::string initQA(const char* args);
        std::string startQA(const char* args);
        std::string stopQA(const char* args);
        std::string shutdownQA(const char* args);
        std::string getQAState(const char* args);
        std::string getQATotalEvent(const char* args);
        std::string getQACurrentEventID(const char* args);
        
};
#endif