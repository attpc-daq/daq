#include "Pad.h"
ClassImp(Pad);
Pad::Pad(){
    padRow = 0;
    padColumn = 0;
    padNumber = 0;
    adc = 0;
    ChargeDeposited = 0;
    Energy = 0;
    DriftTime = 0;
}
Pad::~Pad(){}