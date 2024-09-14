#include "PacketDecoder.h"
#include <TMessage.h>
#include <TH1D.h>

#include <iostream>
#include <fstream>
#include <thread>
using namespace std;

PacketDecoder::PacketDecoder(){
  packetType = notPacket;
  packetSize = 0;
  packetPose = 0;
  firstEvent = true;
  waveformFillStatusCode = 0;
  rawEvent = new RawEvent();
  event_id = 0;
  new_event_id = 0;
  timestamp = 0;
  new_timestamp = 0;

  eventMarkCount = 0;

  filename = '.';
}

PacketDecoder::~PacketDecoder(){
  delete rawEvent;
}

int PacketDecoder::Fill(char _dataByte, bool debug){
  char dataByte[10];
  dataByte[5] = _dataByte;
  switch(packetType){
    case notPacket:
      if(static_cast<unsigned int>(dataByte[5]) == 0x5a ) {
        //match packet header
        packetPose = 1;
        packetType = unknownPacket;
      }
      break;
    case unknownPacket:
      packetSize = (static_cast<unsigned int>(dataByte[5])&0b00011111)<<8;
      packetPose++;
      if((static_cast<unsigned int>(dataByte[5]) &0xE0) == 0x40){
        packetType = headerPacket;
        // break;
      }else if((static_cast<unsigned int>(dataByte[5]) &0xE0) == 0x0){
        packetType = bodyPacket;
        if(event_id == 0)packetType = notPacket;
        // break;
      }else if((static_cast<unsigned int>(dataByte[5]) &0xE0) == 0x20){
        packetType = endPacket;
        if(event_id == 0)packetType = notPacket;
        // break;
      }else{
        //miss match
        packetType = notPacket;
        if(static_cast<unsigned int>(dataByte[5]) == 0x5a ) {
          packetPose = 1;
          packetType = unknownPacket;
        }
        // break;
      }
      break;
    case headerPacket:
      packetPose++;
      switch (packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(dataByte[5]);
          if(packetSize != 20){
            // cout<<"Error: head packet size:"<<packetSize<<" from:"<<filename.c_str()<<endl;
            packetType = notPacket;
          }
          break;
        case 4:
          FEE_ID = static_cast<unsigned int>(dataByte[5])&0b00111111;
        case 5://new_timestamp 47-40 bits
          new_timestamp = 0;
          new_timestamp = (new_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 6://new_timestamp 39-32 bits
          new_timestamp = (new_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 7://new_timestamp 31-24 bits
          new_timestamp = (new_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 8://new_timestamp 23-16 bits
          new_timestamp = (new_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 9://new_timestamp 15-8 bits
          new_timestamp = (new_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 10://new_timestamp 7-0 bits
          new_timestamp = (new_timestamp<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 11://event id 31-24 bits
          new_event_id = 0;
          new_event_id = (new_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 12://event id 23-16 bits
          new_event_id = (new_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 13://event id 15-8 bits
          new_event_id = (new_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 14://event id 7-0 bits
          new_event_id = (new_event_id<<8) |(static_cast<uint64_t>(dataByte[5]) & 0xff);
          break;
        case 15://hit count
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
          // cout<<"file:"<<filename.c_str()<<" new header:"<<new_event_id<<"event marker"<<eventMarkCount<<endl;
          packetType = notPacket;
          eventMarkCount++;
          if(event_id == 0){
            event_id = new_event_id;
            timestamp = new_timestamp;
            rawEvent->event_id = event_id;
            rawEvent->timestamp = timestamp;
          }else if(new_event_id > event_id){
            rawEvent->event_id = event_id;
            rawEvent->timestamp = timestamp;
            event_id = new_event_id;
            timestamp = new_timestamp;
            if(eventMarkCount != 1){
              rawEvent->mark_count = eventMarkCount - 1;
            //   cout<<"ignor:"<<"file:"<<filename.c_str()<<" event:"<<rawEvent->event_id<<" ch "<<size(rawEvent->channels)<<" next id:"<<new_event_id<<endl;
            //   rawEvent->reset();
            //   eventMarkCount = 1;
            //   return 0;
            }
            // cout<<"file:"<<filename.c_str()<<" event:"<<rawEvent->event_id<<" ch "<<size(rawEvent->channels)<<" next id:"<<new_event_id<<endl;
            return 1;
          }
          break;
      }
      break;
    case bodyPacket:
      packetPose++;
      switch(packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(dataByte[5]);
          if(packetSize != 2060){
            // cout<<"Warning: body packet size:"<<packetSize<<" event id "<<rawEvent->event_id<<" ch "<<size(rawEvent->channels)<<endl;
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
            // if((static_cast<unsigned int>(dataByte[5])&0b11110000)!= 0b10000){
            //   cout<<"Warning: ADC data error"<<endl;
            // }
          }else{
            channel.waveform[waveformFillStatusCode/2] = (channel.waveform[waveformFillStatusCode/2]<<8) | (static_cast<unsigned int>(dataByte[5])&0b11111111);//TODO:与channel数据类型相关
          }
          waveformFillStatusCode++;
          if(waveformFillStatusCode == 2048){
            if(event_id>0){
              channel.event_id = event_id;
              channel.timestamp = timestamp;
              rawEvent->AddChannel(channel);
            }
            packetPose++;
            waveformFillStatusCode = 0;
            // cout<<"file:"<<filename.c_str()<<" event body:"<<event_id<<" ch "<<size(rawEvent->channels)<<endl;
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
          packetType = notPacket;
          break;
      }
      break;
    case endPacket:
      packetPose++;
      switch(packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(dataByte[5]);
          if(packetSize != 12){
            // cout<<"file:"<<filename.c_str()<<"Warning: end packet size:"<<packetSize<<" event id "<<rawEvent->event_id<<endl;
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
          packetType = notPacket;
          if(event_id>0)eventMarkCount--;
          // cout<<"file:"<<filename.c_str()<<" new end:"<<new_event_id<<"event marker"<<eventMarkCount<<endl;
          break;
      }
      break;
  }
  return 0;
}
RawEvent* PacketDecoder::getRawEvent(){
  return rawEvent;
}
bool PacketDecoder::isFilling(){
  if(packetType == notPacket && rawEvent->event_id == 0) return false;
  return true;
}