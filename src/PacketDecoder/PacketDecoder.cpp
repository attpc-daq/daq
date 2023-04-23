#include "PacketDecoder.h"
#include <iostream>
#include <TMessage.h>
#include <TH1D.h>

ClassImp(PacketDecoder);

PacketDecoder::PacketDecoder(){
  rawEvent.event_id = 0;
  fillStatusCode = 0;
  waveformFillStatusCode = 0;
}

PacketDecoder::~PacketDecoder(){}

bool PacketDecoder::Fill(const char* dataByte){
    switch(fillStatusCode){
      case 0://find event head
        if(*dataByte == 0x5a ) fillStatusCode = 1;
        break;

      case 1:
        if((*dataByte&0b01100000) == 0b01000000 ){
          fillStatusCode = 2;//event head found
          packetSize = uint((*dataByte&0b00011111)<<8);
          rawEvent.reset();
        }else{
          fillStatusCode = 0;//event head not found
        }
        break;

      case 2:
        fillStatusCode = 3;//packet size 0-7 bits
        packetSize += uint(*dataByte);
        if(packetSize != 20)fillStatusCode = 0;
        break;

      case 3:
        fillStatusCode = 4;//FEE ID
        FEE_ID = uint(*dataByte&0b00111111);
        break;

      case 4:
        fillStatusCode = 5;//timestamp 47-40 bits
        //update: by whk
        rawEvent.timestamp = 0;
        rawEvent.timestamp = (rawEvent.timestamp<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 5:
        fillStatusCode = 6;//timestamp 39-32 bits
        //update: by whk
        rawEvent.timestamp = (rawEvent.timestamp<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 6:
        fillStatusCode = 7;//timestamp 31-24 bits
        rawEvent.timestamp = (rawEvent.timestamp<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 7:
        fillStatusCode = 8;//timestamp 23-16 bits
        rawEvent.timestamp = (rawEvent.timestamp<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 8:
        fillStatusCode = 9;//timestamp 15-8 bits
        rawEvent.timestamp = (rawEvent.timestamp<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 9:
        fillStatusCode = 10;//timestamp 7-0 bits
        rawEvent.timestamp = (rawEvent.timestamp<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 10:
        fillStatusCode = 11;//event id 31-24 bits
        //update: by whk
        rawEvent.event_id = 0;
        rawEvent.event_id = (rawEvent.event_id<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 11:
        fillStatusCode = 12;//event id 23-16 bits
        rawEvent.event_id = (rawEvent.event_id<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 12:
        fillStatusCode = 13;//event id 15-8 bits
        rawEvent.event_id = (rawEvent.event_id<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 13:
        fillStatusCode = 14;//event id 7-0 bits
        rawEvent.event_id = (rawEvent.event_id<<8) | ((static_cast<Long64_t>(*dataByte)) & 0xff);
        break;

      case 14:
        fillStatusCode = 15;// hit count
        rawEvent.hit_count = uint(*dataByte&0b00111111);
        //update: by whk
        // if(rawEvent.hit_count != 0)fillStatusCode = 0;
        break;
      //-----
      case 15:
        fillStatusCode = 16;//reserved
        break;
      case 16:
        fillStatusCode = 17;//CRC-32[31-24]
        break;
      case 17:
        fillStatusCode = 18;//CRC-32[23-16]
        break;
      case 18:
        fillStatusCode = 19;//CRC-32[15-8]
        break;
      case 19:
        fillStatusCode = 20;//CRC-32[7-0]
        //update: by whk
        if(rawEvent.hit_count != 0)fillStatusCode = 0;
        break;


      case 20://find channel head or event tail
        if(*dataByte == 0x5a ) fillStatusCode = 21;//find channel head
        break;

      case 21:
        if((*dataByte&0b01100000) == 0b00000000 ){
          fillStatusCode = 22;//channel head found
          packetSize = uint((*dataByte&0b00011111)<<8);
        }else if((*dataByte&0b01100000) == 0b00100000 ){
          fillStatusCode = 34;//event tail found
          //update: by whk
          // return true;
        }else{
          fillStatusCode = 20;//channel head not found
        }
        break;

      case 22:
        fillStatusCode = 23;//packet size 0-7 bits
        packetSize += uint(*dataByte);
        break;

      case 23:
        //TODO:这里识别有错误会产生过大的值，会导致基于FEE-id的计算出错
        fillStatusCode = 24;//FEE ID
        channel.FEE_id = uint(*dataByte&0b00111111);
        break;

      case 24:
        fillStatusCode = 25;//channel id
        channel.channel_id = uint(*dataByte&0b00111111);
        break;

      case 25:
        fillStatusCode = 26;//reserved
        break;

      case 26:
        fillStatusCode = 27;//ADC data 0[11:8]
        channel.waveform[waveformFillStatusCode] = UInt_t((*dataByte&0b1111)<<8);
        break;

      case 27:
        fillStatusCode = 26;//ADC data 0[7:0]
        channel.waveform[waveformFillStatusCode] += UInt_t(*dataByte);
        waveformFillStatusCode++;
        if(waveformFillStatusCode == 1024){
          waveformFillStatusCode = 0;
          fillStatusCode = 28;
          channel.event_id = rawEvent.event_id;
          rawEvent.Fill(channel);
          } 
        break;
      case 28:
        fillStatusCode = 29;//reserved[15:8]
        break;
      case 29:
        fillStatusCode = 30;//reserved[7:0]
        break;
      case 30:
        fillStatusCode = 31;//CRC-32[31-24]
        break;
      case 31:
        fillStatusCode = 32;//CRC-32[23-16]
        break;
      case 32:
        fillStatusCode = 33;//CRC-32[15-8]
        break;
      case 33: 
        fillStatusCode = 20;//CRC-32[7-0]
        break;

      case 34:
        fillStatusCode = 35;//packet size[12-8]
        //TODO:
        break;
      case 35:
        fillStatusCode = 36;//packet size[7-0]
        //TODO:
        break;
      case 36:
        fillStatusCode = 37;//FEE ID
        //TODO:
        break;
      case 37:
        fillStatusCode = 38;//event size[31-24]
        //TODO:
        break;
      case 38:
        fillStatusCode = 39;//event size[23-16]
        //TODO:
        break;
      case 39:
        fillStatusCode = 40;//event size[15-8]
        //TODO:
        break;
      case 40:
        fillStatusCode = 41;//event size[7-0]
        //TODO:
        break;
      case 41:
        fillStatusCode = 42;//CRC-32[31-24]
        break;
      case 42:
        fillStatusCode = 43;//CRC-32[23-16]
        break;
      case 43:
        fillStatusCode = 44;//CRC-32[15-8]
        break;
      case 44: 
        fillStatusCode = 0;//CRC-32[7-0]
        //update: by whk
        return true;
        break;
      
      default:
        break;
    }
  return false;
}
