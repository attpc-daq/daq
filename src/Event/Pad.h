#ifndef PAD_H_
#define PAD_H_
#include "TGClient.h"
#include "TObject.h"

#include <vector>
using namespace std;
class Pad:public TObject{
public:
    Pad();
    Pad(const Pad& pad);
    Pad(Pad* ch);
    ~Pad();
    int padRow;
    int padColumn;
    int padNumber;
    int adc;
    float ChargeDeposited;
    float Energy;
    float DriftTime;
    ClassDef(Pad,1)
};
#endif // PAD_H_