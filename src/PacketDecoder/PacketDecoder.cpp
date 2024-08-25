#include "PacketDecoder.h"
#include <TMessage.h>
#include <TH1D.h>

#include <iostream>
#include <fstream>
#include <thread>
using namespace std;

PacketDecoder::PacketDecoder(){
  packetType = 0;
  packetSize = 0;
  packetPose = 0;
  firstPacket = true;
  firstEvent = false;
  firstHead = true;
  waveformFillStatusCode = 0;

  rawEvent = NULL;
  // rawEvent = new RawEvent();
  // rawEvent->event_id = 0;

  fillStatusCode = 0;
  findpacket = 0;
  findwhat = 0b11111111;
  
  current_event_id = -1;
  // temp_event_id = -1;
  _timestamp = 0;
  temp_timestamp = 0;
  
  // rawEvent->reset();
}

PacketDecoder::~PacketDecoder(){
  if(rawEvent != NULL) delete rawEvent;
}

int PacketDecoder::Fill(char _dataByte){
  if(rawEvent == NULL){
    rawEvent = new RawEvent();
  }
  char dataByte[10];
  dataByte[5] = _dataByte;
  switch(packetType){
    case 0:
      if(static_cast<unsigned int>(dataByte[5]) == 0x5a ) {
        packetPose = 1;
        packetType = 1;
      }
      break;
    case 1:
      packetSize = (static_cast<unsigned int>(dataByte[5])&0b00011111)<<8;
      packetPose++;
      if((static_cast<unsigned int>(dataByte[5]) &0xE0) == 0x40){
        packetType = 2;
        break;
      }else if((static_cast<unsigned int>(dataByte[5]) &0xE0) == 0x0){
        packetType = 3;
        break;
      }else if((static_cast<unsigned int>(dataByte[5]) &0xE0) == 0x20){
        packetType = 4;
        break;
      }else{
        packetType = 0;
        if(static_cast<unsigned int>(dataByte[5]) == 0x5a ) {
          packetPose = 1;
          packetType = 1;
          cout<<"head error: false head"<<endl;
        }
        break;
      }
    case 2:
      packetPose++;
      switch (packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(dataByte[5]);
          if(packetSize != 20){
            cout<<"packet size error: head:"<<packetSize<<endl;
            firstEvent = true;
            firstPacket = true;
            packetType = 0;
            rawEvent->reset();
            return -1;
          }
          break;
        case 4:
          FEE_ID = static_cast<unsigned int>(dataByte[5])&0b00111111;
        case 5://timestamp 47-40 bits
          _timestamp = 0;
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 6://timestamp 39-32 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 7://timestamp 31-24 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 8://timestamp 23-16 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 9://timestamp 15-8 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 10://timestamp 7-0 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 11://event id 31-24 bits
          current_event_id = 0;
          current_event_id = (current_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 12://event id 23-16 bits
          current_event_id = (current_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 13://event id 15-8 bits
          current_event_id = (current_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 14://event id 7-0 bits
          current_event_id = (current_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          if(firstPacket){
            firstPacket = false;
            rawEvent->event_id = current_event_id;
            rawEvent->timestamp = _timestamp;
          }
          if(current_event_id != rawEvent->event_id){
            firstPacket = true;
            if(firstEvent){
              firstEvent = false;
              // cout<<"First event error: "<<current_event_id<<" "<<rawEvent->event_id<<endl;
              rawEvent->reset();
              return 0;
            }
            if(size(rawEvent->channels)>1024){
              cout<<"channels size error: "<<size(rawEvent->channels)<<endl;
              firstEvent = true;
              firstPacket = true;
              packetType = 0;
              rawEvent->reset();
              return -1;
            }
            // cout<<"channels size "<<size(rawEvent->channels)<<endl;
            // if(rawEvent->event_id>100000)cout<<"event id "<<rawEvent->event_id<<" CH "<<rawEvent->NChannel<<endl;
            // cout<<"event id "<<rawEvent->event_id<<"  PID: "<<getpid()<<" thread id: "<<std::this_thread::get_id()<<endl;
            return 1;//finish a event
          }
          break;
        case 15://hit count
          if(firstPacket){
            firstPacket = false;
            rawEvent->event_id = current_event_id;
            rawEvent->timestamp = _timestamp;
          }
          break;
        case 16://reserved
          break;
        case 17://CRC-32[31-24]
          break;
        case 18://CRC-32[23-16]
          break;
        case 19://CRC-32[15-8]
          break;
        case 20://CRC-32[7-0]
          packetType = 0;
          break;
      }
      break;
    case 3:
      packetPose++;
      switch(packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(dataByte[5]);
          if(packetSize != 2060){
            cout<<"packetSize error, Body:"<<packetSize<<endl;
            firstEvent = true;
            firstPacket = true;
            packetType = 0;
            rawEvent->reset();
            return -1;
          }
          break;
        case 4:
          channel.FEE_id = static_cast<unsigned int>(dataByte[5])&0b00111111;
          break;
        case 5://channel index
          channel.channel_id = static_cast<unsigned int>(dataByte[5])&0b00111111;
          break;
        case 6:
          break;
        case 7:
          packetPose--;
          if(waveformFillStatusCode%2 == 0){
            channel.waveform[waveformFillStatusCode/2] = static_cast<unsigned int>(dataByte[5])&0b1111;//TODO:与channel数据类型相关
            if((static_cast<unsigned int>(dataByte[5])&0b11110000)!= 0b10000){
              firstEvent = true;
              firstPacket = true;
              packetType = 0;
              rawEvent->reset();
              cout<<"ADC data error"<<endl;
              return -1;
            }
          }else{
            channel.waveform[waveformFillStatusCode/2] = (channel.waveform[waveformFillStatusCode/2]<<8) | (static_cast<unsigned int>(dataByte[5])&0b11111111);//TODO:与channel数据类型相关
          }
          waveformFillStatusCode++;
          if(waveformFillStatusCode == 2048){
            channel.event_id = current_event_id;
            channel.timestamp = _timestamp;
            rawEvent->AddChannel(channel);
            packetPose++;
            waveformFillStatusCode = 0;
          }
          break;
        case 8://reserved[15:8]
          break;
        case 9://reserved[7:0]
          break;
        case 10://CRC-32[31-24]
          break;
        case 11://CRC-32[23-16]
          break;
        case 12://CRC-32[15-8]
          break;
        case 13://CRC-32[7-0]
          packetType = 0;
          break;
      }
      break;
    case 4:
      packetPose++;
      switch(packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(dataByte[5]);
          if(packetSize != 12){
            cout<<"packet size error, End:"<<packetSize<<endl;
            firstEvent = true;
            firstPacket = true;
            packetType = 0;
            rawEvent->reset();
            return -1;
          }
          break;
        case 4://FEE ID
          break;
        case 5://event size[31:24]
          break;
        case 6://event size[23:16]
          break;
        case 7://event size[15:8]
          break;
        case 8://event size[7:0]
          break;
        case 9://CRC-32[31-24]
          break;
        case 10://CRC-32[23-16]
          break;
        case 11://CRC-32[15-8] 
          break;
        case 12://CRC-32[7-0]
          packetType = 0;
          break;
      }
      break;
  }
  return 0;
}
RawEvent* PacketDecoder::getRawEvent(){
  RawEvent* ptr = rawEvent;
  rawEvent = NULL;
  return ptr;
}