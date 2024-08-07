#include "EventConverter.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

EventConverter::EventConverter(){
    WValue = 26.0; //unit: eV  气体平均电离能
    Vdrift = 0.05; //unit: mm/ns  电子漂移速度
    PadMap();
    event = new Event();
}
EventConverter::EventConverter(const char* name){
  WValue = 26.0; //unit: eV  气体平均电离能
  Vdrift = 0.05; //unit: mm/ns  电子漂移速度
  PadMap();
  event = new Event();
  ifstream file(name);
  string content((istreambuf_iterator<char>(file)),(istreambuf_iterator<char>()));
  file.close();
  updateSettings(content.c_str());
}

EventConverter::~EventConverter(){
    delete event;
}

void EventConverter::updateSettings(const char* jsonString){
  json json_object = json::parse(jsonString);
  WValue = json_object["WValue"];
  Vdrift = json_object["Vdrift"];
  // std::cout<<"WValue: "<<WValue<<" eV   Vdrift: "<<Vdrift<<" mm/ns"<<std::endl;
  // std::cout << "FPC2:" << std::endl;
  for (const auto& elem : json_object["FPC2"]) {
      for (const auto& [key, value] : elem.items()) {
          int int_key = std::stoi(key);
          // std::cout << int_key << ": " << value << std::endl;
          FPC2[int_key] = value;
      }
  }

  // std::cout << "Micromegas_Gain:" << std::endl;
  for (const auto& elem : json_object["Micromegas_Gain"]) {
      for (const auto& [key, value] : elem.items()) {
          int int_key = std::stoi(key);
          // std::cout << int_key << ": " << value << std::endl;
          Micromegas_Gain[int(int_key/64)][int_key-int(int_key/64)*64] = value;
      }
  }
  // std::cout << "Electronic_Gain:" << std::endl;
  for (const auto& elem : json_object["Electronic_Gain"]) {
      for (const auto& [key, value] : elem.items()) {
          int int_key = std::stoi(key);
          // std::cout << int_key << ": " << value << std::endl;
          Electronic_Gain[int(int_key/64)][int_key-int(int_key/64)*64] = value;
      }
  }
  // std::cout << "Electronic_time_offset:" << std::endl;
  for (const auto& elem : json_object["Electronic_time_offset"]) {
      for (const auto& [key, value] : elem.items()) {
          int int_key = std::stoi(key);
          // std::cout << int_key << ": " << value << std::endl;
          Electronic_time_offset[int(int_key/64)][int_key-int(int_key/64)*64] = value;
      }
  }
}

Event* EventConverter::convert(RawEvent &REvt){
    event->reset();
    event->event_id = REvt.event_id;
    event->timestamp = REvt.timestamp;
    event->WValue = WValue;
    event->Vdrift = Vdrift;
    for(auto iter = REvt.channels.begin(); iter != REvt.channels.end(); ++iter){
        if(iter->FEE_id>=32 || iter->channel_id>64)continue;
        Pad pad;
        Int_t padRow = 0;
        Int_t padColumn = 0;
        int ch = iter->channel_id;
        if(iter->FEE_id%2==0){
          padRow = XPos[FPC2[iter->FEE_id]*128+iter->channel_id];
          padColumn = ZPos[FPC2[iter->FEE_id]*128+iter->channel_id];
        }
        else {
          padRow = XPos[FPC2[iter->FEE_id]*128+(iter->channel_id+64)];
          padColumn = ZPos[FPC2[iter->FEE_id]*128+(iter->channel_id+64)];
        }
        pad.padRow = padRow; 
        pad.padColumn = padColumn;
        
        pad.padNumber = padRow*64+padColumn;

        //计算基线,取前15个点的平均值
        Int_t baseline=(iter->waveform[0]+iter->waveform[1]+iter->waveform[2]+iter->waveform[3]+iter->waveform[4]+iter->waveform[5]+iter->waveform[6]
                    +iter->waveform[7]+iter->waveform[8]+iter->waveform[9]+iter->waveform[10]+iter->waveform[11]+iter->waveform[12]+iter->waveform[13]+iter->waveform[14])/15;
        //找到波形中最大值的索引 int waveform[1024]
        Int_t n = sizeof(iter->waveform)/sizeof(iter->waveform[0]); // 计算数组长度
        Int_t maxIndex = distance(iter->waveform, max_element(iter->waveform, iter->waveform+n)); // 找到最大值索引
        UInt_t max_waveform = iter->waveform[maxIndex];
        //寻找最低值，最小值小于555的一般为噪声去除
        Int_t minIndex = distance(iter->waveform, min_element(iter->waveform, iter->waveform+n)); // 找到最大值索引
        UInt_t min_waveform = iter->waveform[minIndex];
        //对于目前阳极板的测试而言，最大值小于555的一般为噪声去除
        // if(min_waveform<555)continue;
        Int_t ADC = max_waveform - baseline;
        // cout<<"Event id: "<<REvt.event_id<<" ADC: "<<ADC<<" baseline: "<<baseline<<" max_waveform: "<<max_waveform<<" Electronic_Gain[iter->FEE_id][ch]: "<<Electronic_Gain[iter->FEE_id][ch]<<" "<<iter->FEE_id<<" "<<ch<<endl;
        //小于575一般是噪声过阈值
        // if(iter->FEE_id !=3 && iter->channel_id !=38){if(max_waveform<575||ADC<10)continue;}
        Int_t Q = round(ADC/Electronic_Gain[iter->FEE_id][iter->channel_id])*1e-15*6.24150975E+18; //unit:电子数,电子数量必定为整数，所以加round函数
        Float_t En = Q/Micromegas_Gain[padRow][padColumn]*WValue/1E+6; //unit:MeV
        // cout<<"En: "<<En<<" Micromegas_Gain[padRow][padColumn]: "<<Micromegas_Gain[padRow][padColumn]<<" Q: "<<Q<<endl;
        pad.adc = ADC;
        pad.ChargeDeposited = Q;
        pad.Energy = En;

        /*=================================
        **************前沿定时**************
        ===================================*/
        // 找到50%峰值对应的前后两个点的索引,用线性插值计算50%峰值对应的时间
        // pad.DriftTime = iter->timestamp * 8.33 + (maxIndex - 624) * 25;//unit: ns
        const double threshold = ADC*0.5 + baseline;
        int idx1 = 0, idx2 = n - 1;
        for (int i = maxIndex; i >= 0; i--) {
            if (iter->waveform[i] <= threshold) {
                idx1 = i;
                idx2 = i+1;
                break;
            }
        }
        // 计算50%峰值对应的时间
        double t1 = iter->timestamp * 8.33 + (idx1 - 624) * 25;//unit: ns
        double t2 = iter->timestamp * 8.33 + (idx2 - 624) * 25;//unit: ns
        double slope = (threshold - iter->waveform[idx1]) / (iter->waveform[idx2] - iter->waveform[idx1]);
        pad.DriftTime = t1 + slope * (t2 - t1) - Electronic_time_offset[iter->FEE_id][iter->channel_id];//unit: ns
        // std::cout << std::fixed << std::setprecision(0)<<t1<<"  "<<t2<<"  "<<slope<<"  "<<pad.DriftTime<<"  "<<Electronic_time_offset[iter->FEE_id][iter->channel_id]<<endl;

        /*=================================
        **************CFD定时**************
        ===================================*/
        // pad.DriftTime = CFD(iter->waveform, baseline, iter->timestamp, 0.5);

        event->AddPad(pad);
    }
    return event;
}

// Event* EventConverter::convert(RawEvent &REvt){
//     event->reset();
//     event->event_id = REvt.event_id;
//     event->timestamp = REvt.timestamp;
//     event->WValue = WValue;
//     event->Vdrift = Vdrift;

//     Double_t par[4];
//     int time_x[1024];
//     for(int i=0;i<1024;i++)time_x[i] = i*25;

//     for(auto iter = REvt.channels.begin(); iter != REvt.channels.end(); ++iter){
//         if(iter->FEE_id>=32 || iter->channel_id>64)continue;
//         Pad pad;
//         Int_t padRow = 0;
//         Int_t padColumn = 0;
//         int ch = iter->channel_id;
//         if(iter->FEE_id%2==0){
//           padRow = XPos[FPC2[iter->FEE_id]*128+iter->channel_id];
//           padColumn = ZPos[FPC2[iter->FEE_id]*128+iter->channel_id];
//         }
//         else {
//           padRow = XPos[FPC2[iter->FEE_id]*128+(iter->channel_id+64)];
//           padColumn = ZPos[FPC2[iter->FEE_id]*128+(iter->channel_id+64)];
//         }
//         pad.padRow = padRow; 
//         pad.padColumn = padColumn;
        
//         pad.padNumber = padRow*64+padColumn;

//         //找到波形中最大值的索引 int waveform[1024]
//         Int_t n = sizeof(iter->waveform)/sizeof(iter->waveform[0]); // 计算数组长度
//         //寻找最低值，最小值小于555的一般为噪声去除
//         Int_t minIndex = distance(iter->waveform, min_element(iter->waveform, iter->waveform+n)); // 找到最大值索引
//         UInt_t min_waveform = iter->waveform[minIndex];
//         //对于目前阳极板的测试而言，最大值小于555的一般为噪声去除
//         if(min_waveform<555)continue;
        
//         TGraph *gr = new TGraph(1024, time_x, iter->waveform);
//         gr->SetLineColor(kBlack);
//         gr->GetXaxis()->SetTitle("Time (ns)");
//         gr->GetYaxis()->SetTitle("ADC");

//         TF1 *func = new TF1("CR-RC3",CRRCn,0,800*25,4);
//         func->SetLineColor(kRed);
//         func->SetLineWidth(2);
//         func->SetParameter(0,600*25);func->SetParName(0,"t0");
//         func->SetParameter(1,1000);func->SetParName(1,"tau");
//         func->SetParName(2,"A");
//         func->SetParameter(3,570);func->SetParName(3,"B");
//         //第一次拟合，得到初步的参数，取得最大值和基线
//         gr->Fit("CR-RC3","RQ");
//         func->GetParameters(&par[0]);

//         func->SetRange(0,func->GetMaximumX());
//         func->SetParameters(par);
//         gr->Fit("CR-RC3","RQ");
//         func->GetParameters(&par[0]);

//         //第二次拟合，取得最大值和基线以及t0
//         //t0 = par[0]; baseline = par[3]; max_waveform = func->GetMaximum();
//         double t = iter->timestamp*8.33+(par[0]-624*25);
//         double max_waveform = func->GetMaximum();
//         double ADC = max_waveform-par[3];

//         //小于575一般是噪声过阈值
//         if(iter->FEE_id !=3 && iter->channel_id !=38){if(max_waveform<575||ADC<10)continue;}
//         Double_t Q = round(ADC/Electronic_Gain[iter->FEE_id][iter->channel_id])*1e-15*6.24150975E+18; //unit:电子数,电子数量必定为整数，所以加round函数
//         Float_t En = Q/Micromegas_Gain[padRow][padColumn]*WValue/1E+6; //unit:MeV
//         // cout<<"En: "<<En<<" Micromegas_Gain[padRow][padColumn]: "<<Micromegas_Gain[padRow][padColumn]<<" Q: "<<Q<<endl;
//         pad.adc = ADC;
//         pad.ChargeDeposited = Q;
//         pad.Energy = En;
//         pad.DriftTime = t - Electronic_time_offset[iter->FEE_id][iter->channel_id];//unit: ns

//         event->AddPad(pad);
//     }
//     return event;
// }

// Double_t EventConverter::CRRCn(Double_t *x, Double_t *par)
// {
//    int n = 3;
//    Double_t t0 = par[0];
//    Double_t tau = par[1];
//    Double_t A = par[2];
//    Double_t B = par[3];
//    Double_t t = x[0]-t0;
//    Double_t result = 0;
//    //heavisde step function
//    if(t<0)result = B;
//    else result = A*exp(-t/tau)*pow(t/tau,n)+B;
//    return result;
// }

// double EventConverter::CFD(UInt_t* waveform, Int_t baseline, uint64_t timestampe, double attenuate_factor){
//     double delaytime[1024];
//     UInt_t attenuateAndinvert[1024];
//     double T0=0;//
//     UInt_t CFD_waveform[1024];


//     int maxIndex,minIndex=0;
//     maxIndex = distance(waveform, max_element(waveform, waveform+1024)); // 找到最大值索引
//     UInt_t max_waveform = (UInt_t)waveform[maxIndex];
//     int ADC = max_waveform - baseline;
//     const double Tr_90 = ADC*0.9 + baseline;
//     const double Tr_10 = ADC*0.1 + baseline;
//     //计算从10%到90%的时间
//     int idx1;
//     for(int j=maxIndex;j>0;j--){
//         if(waveform[j]<Tr_90){
//             idx1=j;
//             break;
//         }
//     }
//     int idx2;
//     for(int j=idx1;j>0;j--){
//         if(waveform[j]<Tr_10){
//             idx2=j;
//             break;
//         }
//     }
//     int Tr=(idx1-idx2)*25;
//     double Td=Tr*(1-attenuate_factor);

//     for(int j=0;j<1024;j++){
//         attenuateAndinvert[j]=baseline-attenuate_factor*(waveform[j]-baseline);
//         delaytime[j]=timestampe * 8.33 + (j-624) * 25 + Td;
//         CFD_waveform[j]=baseline;
//     }

//     for(int j=0;j<1024;j++){
//         int index = j+Td/25;
//         if(index>=1024)continue;
//         CFD_waveform[j]=waveform[j]+attenuateAndinvert[index]-baseline;
//     }

//      //寻找CFD_waveform最大与最小值，并求出两者之间过baseline的值
//     int n_cfd = sizeof(CFD_waveform)/sizeof(CFD_waveform[0]); // 计算数组长度
//     maxIndex = distance(CFD_waveform, max_element(CFD_waveform, CFD_waveform+n_cfd)); // 找到最大值索引
//     minIndex = distance(CFD_waveform, min_element(CFD_waveform, CFD_waveform+n_cfd)); // 找到最小值索引

//     // cout<<i<<"  maxIndex: "<<maxIndex<<"  max: "<<CFD_waveform[maxIndex]<<"  minIndex: "<<minIndex<<"  min: "<<CFD_waveform[minIndex]<<endl;
//     for(int j=minIndex;j<maxIndex;j++){
//         //线性差值求过baseline的时间
//         if(CFD_waveform[j]<=baseline&&CFD_waveform[j+1]>=baseline){
//             T0=(delaytime[j+1]-delaytime[j])/(CFD_waveform[j+1]-CFD_waveform[j])*(baseline-CFD_waveform[j])+delaytime[j];
//             // cout<<"T0: "<<T0<<" ns"<<endl;
//             break;
//         }
//     }

//     return T0;
// }

void EventConverter::PadMap(){
    int IDD[200];

    int PAD[32][64];
    int PAD1[32][64];
    int PAD2[32][64];
    int PAD3[32][64];
    int PADR[32][64];
    
    //================================IDmatch2(I1, I2);
    int I1,I2;
    int K1=0;

    for(int iz=0;iz<64;iz++)
    {
      for(int ix=0;ix<32;ix++)
      {
        PAD[ix][iz]=-1;
        PAD1[ix][iz]=-1;
        PAD2[ix][iz]=-1;
        PAD3[ix][iz]=-1;
      }
    }
 

    for(int i=0;i<128;i++)
    {
      if(i< 64){
        K1=i;
        if(i%2==0){
          if(i%4==0){I1=15-K1/4;I2=16+0;}
          if(i%4==2){I1=15-K1/4;I2=16+2;}
        }
        if(i%2==1){
          if(i%4==1){I1=15-K1/4;I2=16+4;}
          if(i%4==3){I1=15-K1/4;I2=16+6;}
        }
      }

      if(i>=64){
        K1=i-64;
        if(i%2==0){
          if(i%4==0){I1=15-K1/4;I2=16+1;}
          if(i%4==2){I1=15-K1/4;I2=16+3;}
        }
        if(i%2==1){
          if(i%4==1){I1=15-K1/4;I2=16+5;}
          if(i%4==3){I1=15-K1/4;I2=16+7;}
        }
      } 
      PAD[I2][I1]=i; 
    }
 
    for(int iz=0;iz<64;iz++)
      for(int ix=0;ix<32;ix++)PAD1[ix][iz]=PAD[ix][iz];
  
    int III=0;
    //C1==8
    for(int iz=0;iz<64;iz++)
    {
      for(int ix=0;ix<32;ix++)
        if(PAD[ix][iz]!=-1)
        {
          PAD1[ix][iz]=PAD[ix][iz];
          PADR[ix][iz]=8;

          PAD1[ix+8][iz]=PAD[ix][iz];
          PADR[ix+8][iz]=9;

          PAD1[ix][iz+16]=PAD[ix][iz];
          PADR[ix][iz+16]=10;

          PAD1[ix+8][iz+16]=PAD[ix][iz];
          PADR[ix+8][iz+16]=11; 
          III+=4;
        }
    }
    //cout<<"III="<<III<<endl;

    for(int iz=0;iz<64;iz++)
      for(int ix=0;ix<32;ix++)PAD2[ix][iz]=PAD1[ix][iz];

    //C1=12,13,14,15
    for(int iz=0;iz<64;iz++)
    {
      for(int ix=0;ix<32;ix++)
        if(PAD1[ix][iz]!=-1)
        {
          PAD2[ix][63-iz]=PAD1[ix][iz];
          if(PADR[ix][iz]==8 )PADR[ix][63-iz]=15; 
          if(PADR[ix][iz]==9 )PADR[ix][63-iz]=14;
          if(PADR[ix][iz]==10)PADR[ix][63-iz]=13;
          if(PADR[ix][iz]==11)PADR[ix][63-iz]=12;
          III++;
        }
    }
    //cout<<"III="<<III<<endl;
  
  
    for(int iz=0;iz<64;iz++)
      for(int ix=0;ix<32;ix++)PAD3[ix][iz]=PAD2[ix][iz];

    //C1=0,1,2,3,4,5,6,7,8
    for(int iz=0;iz<64;iz++)
    {
      for(int ix=0;ix<32;ix++)
        if(PAD2[ix][iz]!=-1)
        {
          PAD3[31-ix][iz]=PAD2[ix][iz];
          PADR[31-ix][iz]=PADR[ix][iz]-8;
          III++;
        }
    }

    int ID11=0;
    for(int i=0;i<16;i++)
    {
      ID11=i;
      //==================================Connector2(ID11,IDD);
      int AID[128];
      int BID[128];
      int GG2[64]={57,41,25,9,49,33,17,1,56,40,24,8,48,32,16,0,61,45,29,13,53,37,21,5,60,44,28,12,52,36,20,4,
               27,11,59,43,19,3,51,35,31,15,63,47,23,7,55,39,26,10,58,42,18,2,50,34,30,14,62,46,22,6,54,38};
      int GG1[64]={57,41,25,9,49,33,17,1,56,40,24,8,48,32,16,0,61,45,29,13,53,37,21,5,60,44,28,12,52,36,20,4,
               27,11,59,43,19,3,51,35,31,15,63,47,23,7,55,39,26,10,58,42,18,2,50,34,30,14,62,46,22,6,54,38};  

      for(int i=0;i<128;i++)IDD[i]=-1;

      if(ID11==8||ID11==9||ID11==10||ID11==11||ID11==4||ID11==5||ID11==6||ID11==7)
      {
        for(int i=0;i<128;i++)
        {
          AID[i]=i;
          if(i< 64)BID[i]=i+64;
          if(i>=64)BID[i]=i-64;
        }
      }


      if(ID11==0||ID11==1||ID11==2||ID11==3||ID11==12||ID11==13||ID11==14||ID11==15)
      {
        for(int i=0;i<128;i++)
        {
          if(i< 64)AID[i]=63 -i;
          if(i>=64)AID[i]=191-i;
        }
        for(int i=0;i<128;i++)
        {
          if(AID[i]< 64)BID[i]=AID[i]+64;
          if(AID[i]>=64)BID[i]=AID[i]-64; 
        }
      }


      for(int i=0;i<128;i++)
      {
        int I1=BID[i];
        if(I1>=0 &&I1< 32)IDD[i]=GG1[I1-0 ]+64;   //B board
        if(I1>=32&&I1< 64)IDD[i]=GG2[I1-32]+0 ;   //A Board
        if(I1>=64&&I1< 96)IDD[i]=GG1[I1-32]+64;   //B board
        if(I1>=96&&I1<128)IDD[i]=GG2[I1-64]+0 ;   //A Board
      }
      //====================
      for(int ix=31;ix>=0;ix--)
      {
        for(int iz=0;iz<64;iz++)
        {
          if(PADR[ix][iz]==ID11)
          {
            ZPos[i*128+IDD[PAD3[ix][iz]]]=iz;
            XPos[i*128+IDD[PAD3[ix][iz]]]=ix;
          }
        }
      } 
    }
}