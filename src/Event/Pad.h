#ifndef PAD_H_
#define PAD_H_
#include "TGClient.h"
#include "TObject.h"

#include <vector>
using namespace std;
class Pad:public TObject{
public:
    Pad();
    Pad(const Pad& other);
    Pad(Pad* other);
    ~Pad();
    int padRow;
    int padColumn;
    int padNumber;
    double adc;
    double ChargeDeposited;
    float Energy;
    double DriftTime;
    ClassDef(Pad,1)
};
#endif // PAD_H_