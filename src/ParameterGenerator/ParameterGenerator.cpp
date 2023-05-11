#include "ParameterGenerator.h"
#include "TMath.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

ClassImp(ParameterGenerator);

ParameterGenerator::ParameterGenerator(){
    reset();
}
ParameterGenerator::~ParameterGenerator(){
}
void ParameterGenerator::reset(){
    for(int i = 0;i<32;i++){
            for(int j = 0; j<64;j++){
                sum_threshold[i][j]=0;
                count[i][j]=0;
            }
        }
}
void ParameterGenerator::fill(RawEvent* revt){
    for(int i=0;i<revt->NChannel;i++){
        Channel* ch = (Channel*) revt->channels->At(i);
        int len = sizeof(ch->waveform)/sizeof(UInt_t);
        int waveform_mean = TMath::Mean(len, ch->waveform);
        int waveform_rms = TMath::RMS(len, ch->waveform);
        int threshold = waveform_mean+ 20* waveform_rms;
        if(ch->FEE_id > 31) continue;
        if(ch->channel_id>63)continue;
        sum_threshold[ch->FEE_id][ch->channel_id]+=threshold;
        count[ch->FEE_id][ch->channel_id]+=1;
    }
}
string ParameterGenerator::getThreshold(){
    json js={};
    uint threshold[32][64];
    for(int i = 0;i<32;i++){
        for(int j = 0; j<64;j++){

            if(sum_threshold[i][j]==0)sum_threshold[i][j]=580;
            if(count[i][j]==0)count[i][j]=1;
            threshold[i][j] = sum_threshold[i][j]/count[i][j];

            json msg;

            stringstream cmd1;
            cmd1    << "06102831" << "4"
                    << std::hex << i % 16 << "5"
                    << std::hex << std::floor(i / 16) << "607083";

            msg.push_back({{"FE_id",cmd1.str()}});

            stringstream cmd2;
            cmd2    << "08102831" << "4"
                    << std::hex << j % 16 << "5"
                    << std::hex << std::floor(j / 16) << "607083";

            msg.push_back({{"channel_id",cmd2.str()}});

            uint threshold_value = threshold[i][j];
            stringstream cmd3;
            cmd3  << "0A102831" << "4"
                        << std::hex << threshold_value % 16 << "5"
                        << std::hex << (threshold_value / 16) % 16 << "6"
                        << std::hex << ((threshold_value / 16) / 16) % 16 << "7"
                        << std::hex << (((threshold_value / 16) / 16) / 16) % 16
                        << "83";
            msg.push_back({{"threshold",cmd3.str()}});

            string cmd4 = "0C1028314150607083";
            msg.push_back({{"end",cmd4}});
            js.update({{to_string(i*64+j),msg}});
        }
        js.update({{"count",int(32*64)}});
    }
    return js.dump(2);
}

string ParameterGenerator::getSettings(int WValue, int Vdrift) {
    // 创建 FPC2 数组
    std::vector<std::map<std::string, int>> FPC2 = {
        {{"0", 2}},
        {{"1", 2}},
        {{"2", 0}},
        {{"3", 0}},
        {{"4", 10}},
        {{"5", 10}},
        {{"6", 8}},
        {{"7", 8}},
        {{"8", 15}},
        {{"9", 15}},
        {{"10", 15}},
        {{"11", 15}},
        {{"12", 15}},
        {{"13", 15}},
        {{"14", 15}},
        {{"15", 15}},
        {{"16", 15}},
        {{"17", 15}},
        {{"18", 15}},
        {{"19", 15}},
        {{"20", 15}},
        {{"21", 15}},
        {{"22", 15}},
        {{"23", 15}},
        {{"24", 15}},
        {{"25", 15}},
        {{"26", 15}},
        {{"27", 15}},
        {{"28", 15}},
        {{"29", 15}},
        {{"30", 15}},
        {{"31", 15}}
    };

    // 创建 Micromegas_Gain 数组
    std::vector<std::map<std::string, double>> Micromegas_Gain(2048);
    for (int i = 0; i < 2048; i++) {
        Micromegas_Gain[i][std::to_string(i)] = 1000.0;
    }

    //创建 Electronic_Gain 数组
    std::vector<std::map<std::string, double>> Electronic_Gain(2048);
    for (int i = 0; i < 2048; i++) {
        Electronic_Gain[i][std::to_string(i)] = 0.75;
    }

    // 创建整个 JSON 对象
    json json_data = {
        {"WValue", WValue},
        {"Vdrift", Vdrift},
        {"FPC2", FPC2},
        {"Micromegas_Gain", Micromegas_Gain},
        {"Electronic_Gain", Electronic_Gain}
    };

    // 将 JSON 对象转换为字符串
    std::string eventSettingJson = json_data.dump(2);
    return eventSettingJson;
}