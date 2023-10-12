#include "ParameterGenerator.h"
#include "TMath.h"
#include<cmath>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
    for(auto ch = revt->channels.begin(); ch != revt->channels.end(); ++ch){
        int len = sizeof(ch->waveform)/sizeof(int);
        float waveform_mean = TMath::Mean(len, ch->waveform);
        float waveform_rms = TMath::RMS(len, ch->waveform);
        int threshold = round(waveform_mean+ 15* waveform_rms); 
        // int threshold = waveform_mean+ 15;

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

            if(sum_threshold[i][j]==0)sum_threshold[i][j]=590;
            if(count[i][j]==0)count[i][j]=1;
            threshold[i][j] = sum_threshold[i][j]/count[i][j];
            // if(threshold[i][j]<590)threshold[i][j] = 590;
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

// string ParameterGenerator::getSettings(int WValue, int Vdrift, std::vector<std::map<string,int>> FPC2, const char* ElectronicFIle, const char* MicromegasFile) {
   string ParameterGenerator::getSettings(int WValue, int Vdrift, const char* FPC2Buffer, const char* ElectronicFIle, const char* MicromegasFile) {
    //TODO: 根据MicromegasFile读取解析并创建 Micromegas_Gain 数组
    std::vector<std::map<std::string, double>> Micromegas_Gain(2048);
    for (int i = 0; i < 2048; i++) {
        Micromegas_Gain[i][std::to_string(i)] = 1000.0;
    }

    //TODO: 根据ElectronicFIle读取解析并创建 Electronic_Gain 数组
    std::vector<std::map<std::string, double>> Electronic_Gain(2048);
    std::vector<std::map<std::string, double>> Electronic_time_offset(2048);
    //默认赋值
    for (int i = 0; i < 2048; i++) {
        Electronic_Gain[i][std::to_string(i)] = 1.0;
        Electronic_time_offset[i][std::to_string(i)] = 0.0;
    }
    
    std::ifstream file(ElectronicFIle);
    if (!file.is_open() || !file.good()) {
        std::cout << "Error opening file " << ElectronicFIle << std::endl;
        exit(1);
    }

    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();

    json js = json::parse(content);

    for (const auto& elem : js["Electronic_Gain"]) {
        // std::map<std::string, double> gainEntry;
        for (const auto& [key, value] : elem.items()) {
            // gainEntry[key] = value;
            Electronic_Gain[std::stoi(key)][key] = value;
        }
        // Electronic_Gain.push_back(gainEntry);
    }

    for (const auto& elem : js["Electronic_time_offset"]) {
        // std::map<std::string, double> offsetEntry;
        for (const auto& [key, value] : elem.items()) {
            // offsetEntry[key] = value;
            Electronic_time_offset[std::stoi(key)][key] = value;
        }
        // Electronic_time_offset.push_back(offsetEntry);
    }
    // 打印读取的数据
    // for (const auto& gainEntry : Electronic_Gain) {
    //     for (const auto& [key, value] : gainEntry) {
    //         std::cout << "Gain: " << key << " -> " << value << std::endl;
    //     }
    // }

    // for (const auto& offsetEntry : Electronic_time_offset) {
    //     for (const auto& [key, value] : offsetEntry) {
    //         std::cout << "Time Offset: " << key << " -> " << value << std::endl;
    //     }
    // }
    // 从buffer中读取FPC2
    json FPC2Json = json::parse(FPC2Buffer);
    std::vector<std::map<string,int>> FPC2;
    for(const auto& element:FPC2Json){
        std::map<string, int> mapData;
        for(const auto& pair:element.items()){
            mapData[pair.key()] = pair.value();
        }
        FPC2.push_back(mapData);
    }
    // 创建整个 JSON 对象
    json json_data = {
        {"WValue", WValue},//unit: eV
        {"Vdrift", Vdrift},//unit: mm/ns
        {"FPC2", FPC2},
        {"Micromegas_Gain", Micromegas_Gain},
        {"Electronic_Gain", Electronic_Gain},
        {"Electronic_time_offset", Electronic_time_offset}
    };

    // 将 JSON 对象转换为字符串
    std::string eventSettingJson = json_data.dump(2);
    return eventSettingJson;
}