#ifndef __ParameterGenerator_h__
#define __ParameterGenerator_h__
#include "TGClient.h"
#include "TObject.h"
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

#include <fcntl.h>

using namespace std;

class ParameterGenerator:public TObject {
public:
    ParameterGenerator();
    virtual ~ParameterGenerator();
    void reset();
    void fill(RawEvent* revt);
    string getSettings(int WValue, int Vdrift);
    string getThreshold();

private:
    
    uint sum_threshold[32][64];
    uint count[32][64];

    RawEvent rawEvent;
    ClassDef(ParameterGenerator,1)
};

#endif
