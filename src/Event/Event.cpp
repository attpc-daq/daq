#include "Event.h"
//#include "RawToPad.h"

#include <iostream>
using namespace std;

ClassImp(Event);

Event::Event(){
    event_id = 0;
    WValue = 30.0;
    Vdrift = 0;
    vector<Pad>().swap(pads);
}

// void Event::fill(RawEvent *REvt){
//     for (auto iter : REvt->channels)
//     {
//         Pad pad;
//         int padRow = XPos[FPC2[iter.FEE_id]*128+iter.channel_id];
//         int padColumn = ZPos[FPC2[iter.FEE_id]*128+iter.channel_id];
//         pad.padRow = padRow; 
//         pad.padColumn = padColumn;
//         pad.padNumber = padRow*64+padColumn;

//         //计算基线,取前15个点的平均值
//         int baseline=(iter.waveform[0]+iter.waveform[1]+iter.waveform[2]+iter.waveform[3]+iter.waveform[4]+iter.waveform[5]+iter.waveform[6]
//                     +iter.waveform[7]+iter.waveform[8]+iter.waveform[9]+iter.waveform[10]+iter.waveform[11]+iter.waveform[12]+iter.waveform[13]+iter.waveform[14])/15;
//         //找到波形中最大值的索引
//         int n = sizeof(iter.waveform)/sizeof(iter.waveform[0]); // 计算数组长度
//         int maxIndex = distance(iter.waveform, max_element(iter.waveform, iter.waveform+n)); // 找到最大值索引
//         UInt_t max_waveform = (UInt_t)iter.waveform[maxIndex];  
//         //小于570一般是噪声过阈值
//         if(max_waveform<570)return false;
//         int ADC = max_waveform - baseline;
//         int Q = round(ADC/Electronic_Gain[padRow][padColumn])*1e-15*6.24150975E+18; //unit:电子数
//         float En = Q/Pad_Gain[padRow][padColumn]*WValue/1E+6; //unit:MeV
//         pad.ChargeDeposited = Q;
//         pad.Energy = En;
//         pad.DriftTime = iter.timestamp*8.33+(maxIndex+1 - 624)*25; //unit: ns

//         pads.push_back(pad);
//     }
    
// }

Event::~Event(){}
