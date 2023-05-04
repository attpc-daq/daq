#include "ParameterGenerator.h"
#include "TMath.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;


ClassImp(ParameterGenerator);

ParameterGenerator::ParameterGenerator(){
    readingTempFileName = "reading";
    for(int i = 0;i<32;i++){
        for(int j = 0; j<64;j++){
            sum_threshold[i][j]=0;
            count[i][j]=0;
        }
    }
}
ParameterGenerator::~ParameterGenerator(){
}
void ParameterGenerator::setRawDataDir(const char* dir){
    rawDataDir = dir;
    if(dir[rawDataDir.length()-1]!='/')rawDataDir.append("/");
    std::filesystem::create_directory(rawDataDir.c_str());
}
void ParameterGenerator::setOutputDir(const char* dir){
    outputDir = dir;
    if(dir[outputDir.length()-1]!='/')outputDir.append("/");
    std::filesystem::create_directory(outputDir.c_str());
}

void ParameterGenerator::setRawDataFilePrefix(const char* rawfileprefix){
    rawDataFilePrefix = rawfileprefix;
}
void ParameterGenerator::setOutputFileName(const char* thrName, const char* evtParName){
    thresholdFileName = thrName;
    eventParameterFileName = evtParName;
}
    
void ParameterGenerator::setReadingTempFileName(const char* name){
    readingTempFileName = name;
}
int ParameterGenerator::state(){ return status; }
bool ParameterGenerator::isFinish(){
    if(status == status_stopped){
        return true;
    }
    return false;
}

bool ParameterGenerator::openRawDataFile(){
    updateRawDataFileID();
    if(rawDataFileID<0)return false;
    int check = rename (( rawDataDir+rawDataFilePrefix+to_string(rawDataFileID)+".dat").c_str(), (rawDataDir+readingTempFileName).c_str() );
    if(check == -1) return false;
    rawDataFile.open((rawDataDir+readingTempFileName).c_str(), std::ios::binary);
    if (!rawDataFile.is_open()) {
        return false;
    }
    return true;
}
void ParameterGenerator::closeRawDataFile(){
    rawDataFile.close();
}
void ParameterGenerator::updateRawDataFileID(){
    rawDataFileID = -1;
    for (const auto & file : std::filesystem::directory_iterator(rawDataDir)){
        string ss(file.path());
        if(strstr(ss.c_str(),rawDataFilePrefix.c_str())==NULL)continue;
        int bit = 0;
        int id = 0;
        int i = ss.length()-5;
        while(i>0){
            if(ss[i]<48 || ss[i]>57)break;
            id += (ss[i]-48)*pow(10,bit);
            bit++; 
            i--;
        }
        if((id<rawDataFileID)||(rawDataFileID == -1))rawDataFileID = id;
    }
}

void ParameterGenerator::stop(){
    status = status_stopping;
    mThread->join();
}
void ParameterGenerator::run(int nEvent){
    mThread = new thread(&ParameterGenerator::mLoop, this, nEvent);
}

void ParameterGenerator::mLoop(int nEvent){
    cout<<"parameter generation loop start"<<endl;
    status = status_running;
    char byte;
    int eventCount = 0;
    while(status == status_running){
        if(!openRawDataFile()){
            sleep(1);
            continue;
        }
        while (rawDataFile.read(&byte, 1)) {
            //UPDATE: by whk
            if(decoder.Fill(&byte)){
                rawEvent = decoder.rawEvent;
                eventCount++;
                cout<<"events for parameter generation "<<eventCount<<" / "<<nEvent<<endl;
                fill(&rawEvent);
                if(eventCount>nEvent){
                    status = status_stopping;
                    break;
                }
                //UPDATE: by whk
                decoder.rawEvent.reset();
            }
        }
        closeRawDataFile();
    }
    make_threshold();
    status = status_stopped;
    cout<<"parameter generator done"<<endl;
}
void ParameterGenerator::fill(RawEvent* revt){
    int size = revt->channels.size();
    for(int i= 0;i<size;i++){
        int len = sizeof(revt->channels[i].waveform)/sizeof(UInt_t);
        int waveform_mean = TMath::Mean(len, revt->channels[i].waveform);
        int waveform_rms = TMath::RMS(len, revt->channels[i].waveform);
        int threshold = waveform_mean+ 20* waveform_rms;
        if(revt->channels[i].FEE_id > 31) continue;
        if(revt->channels[i].channel_id>63)continue;
        sum_threshold[revt->channels[i].FEE_id][revt->channels[i].channel_id]+=threshold;
        count[revt->channels[i].FEE_id][revt->channels[i].channel_id]+=1;
    }
}
void ParameterGenerator::make_threshold(){
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
    // string content = js.dump(2);
    ofstream file1((outputDir+thresholdFileName).c_str());
    file1<<js.dump(2);
    file1.close();
    ofstream file2((outputDir+eventParameterFileName).c_str());
    file2<<generate_settings(33.2,10);
    file2.close();
}

string ParameterGenerator::generate_settings(int WValue, int Vdrift) {
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