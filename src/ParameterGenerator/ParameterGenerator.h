#ifndef __ParameterGenerator_h__
#define __ParameterGenerator_h__

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <atomic>
#include "RawEvent.h"

#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h>

using namespace std;

class ParameterGenerator{
public:
    ParameterGenerator();
    virtual ~ParameterGenerator();
    void reset();
    void fill(RawEvent* revt);
    // string getSettings(int WValue, int Vdrift);
    // string getSettings(int WValue, int Vdrift, std::vector<std::map<std::string, int>> FPC2, const char* ElectrnicFIle, const char* MicromegasFile);
    string getSettings(float WValue, float Vdrift, const char* FPC2, const char* ElectrnicFIle, const char* MicromegasFile);
    string getThreshold();

private:
    
    float sum_threshold_mean[32][64];
    float sum_threshold_std[32][64];
    uint count[32][64];

    RawEvent rawEvent;
};

#endif
