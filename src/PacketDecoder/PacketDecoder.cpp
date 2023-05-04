#include "PacketDecoder.h"
#include <TMessage.h>
#include <TH1D.h>

#include <iostream>
#include <fstream>
using namespace std;

ClassImp(PacketDecoder);

PacketDecoder::PacketDecoder(){
  packetType = 0;
  packetSize = 0;
  packetPose = 0;
  firstPacket = true;
  firstEvent = true;
  firstHead = true;
  waveformFillStatusCode = 0;

  rawEvent.event_id = 0;

  fillStatusCode = 0;
  findpacket = 0;
  findwhat = 0b11111111;
  
  _event_id = -1;
  temp_event_id = -1;
  _timestamp = 0;
  temp_timestamp = 0;
  rawEvent.reset();
}

PacketDecoder::~PacketDecoder(){}

// bool PacketDecoder::Fill(ifstream* file,const char* dataByte){
//   //sleep(0.1);
//   //cout<<"packetType: "<<packetType<< "packetPose: "<<packetPose<<" waveformFillStatusCode: "<<waveformFillStatusCode<<endl;
//   //UPDATE: 寻找第一次出现的header位置
//   char buff[1];
//   if(firstPacket){
//     if(static_cast<unsigned int>(*dataByte) == 0x5a ){
//       file->read((char*)buff,1);
//       if((static_cast<unsigned int>(buff[0]) &0xE0) == 0x40){
//         packetSize = (static_cast<unsigned int>(buff[0])&0b00011111)<<8;
//         file->read((char*)buff,1);
//         packetSize += static_cast<unsigned int>(buff[0]);
//         if(packetSize != 20)goto Nextwhile;
//         packetType = 2;
//         packetPose = 4;
//         firstPacket = false;
//         return false;
//       }
//     }
//     else{
//     Nextwhile:
//       while (file->read((char*)buff,1)){
//         if(static_cast<unsigned int>(buff[0]) != 0x5a ) continue;
//         else{
//           file->read((char*)buff,1);
//           if((static_cast<unsigned int>(buff[0]) &0xE0) == 0x40){
//             packetSize = (static_cast<unsigned int>(buff[0])&0b00011111)<<8;
//             file->read((char*)buff,1);
//             packetSize += static_cast<unsigned int>(buff[0]);
//             if(packetSize != 20)continue;
//             packetType = 2;
//             packetPose = 4;
//             firstPacket = false;
//             return false;
//           }
//           else continue;
//         }
//       }
//     }
//     return false;
//   }

//   switch(packetType){
//     case 0:
//       if(static_cast<unsigned int>(*dataByte) == 0x5a ) {
//         packetPose = 1;
//         packetType = 1;
//       }
//       break;
//     case 1:
//       packetSize = (static_cast<unsigned int>(*dataByte)&0b00011111)<<8;
//       packetPose++;
//       if((static_cast<unsigned int>(*dataByte) &0xE0) == 0x40){
//         packetType = 2;
//         break;
//       }else if((static_cast<unsigned int>(*dataByte) &0xE0) == 0x0){
//         packetType = 3;
//         break;
//       }else if((static_cast<unsigned int>(*dataByte) &0xE0) == 0x20){
//         packetType = 4;
//         break;
//       }else{
//         packetType = 0;
//         break;
//       }
//     case 2:
//       packetPose++;
//       switch (packetPose){
//         case 3:
//           packetSize += static_cast<unsigned int>(*dataByte);
//           if(packetSize != 20)cout<<"header error"<<endl;
//           break;
//         case 4:
//           FEE_ID = static_cast<unsigned int>(*dataByte)&0b00111111;
//         case 5://timestamp 47-40 bits
//           _timestamp = 0;
//           _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 6://timestamp 39-32 bits
//           _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 7://timestamp 31-24 bits
//           _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 8://timestamp 23-16 bits
//           _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 9://timestamp 15-8 bits
//           _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 10://timestamp 7-0 bits
//           _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 11://event id 31-24 bits
//           _event_id = 0;
//           _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 12://event id 23-16 bits
//           _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 13://event id 15-8 bits
//           _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           break;
//         case 14://event id 7-0 bits
//           _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//           if(firstHead){
//             firstHead = false;
//             rawEvent.event_id = _event_id;
//             rawEvent.timestamp = _timestamp;
//           }
//           if(_event_id != rawEvent.event_id){
//             firstHead = true;
//             if(firstEvent){
//               firstEvent = false;
//               rawEvent.reset();
//               return false;
//             }
//             cout<<"event id "<<rawEvent.event_id<<" channels "<<rawEvent.channels.size()<<endl;
//             return true;
//           }
//           break;
//         case 15://hit count
//           if(firstHead){
//             firstHead = false;
//             rawEvent.event_id = _event_id;
//             rawEvent.timestamp = _timestamp;
//           }
//           break;
//         case 16://reserved
//           break;
//         case 17://CRC-32[31-24]
//           break;
//         case 18://CRC-32[23-16]
//           break;
//         case 19://CRC-32[15-8]
//           break;
//         case 20://CRC-32[7-0]
//           packetType = 0;
//           break;
//       }
//       break;
//     case 3:
//       packetPose++;
//       switch(packetPose){
//         case 3:
//           packetSize += static_cast<unsigned int>(*dataByte);
//           if(packetSize != 2060)cout<<"body error: "<<packetSize<<endl;
//           break;
//         case 4:
//           channel.FEE_id = static_cast<unsigned int>(*dataByte)&0b00111111;
//           break;
//         case 5://channel index
//           channel.channel_id = static_cast<unsigned int>(*dataByte)&0b00111111;
//           break;
//         case 6:
//           break;
//         case 7:
//           packetPose--;
//           if(waveformFillStatusCode%2 == 0){
//             channel.waveform[waveformFillStatusCode/2] = static_cast<unsigned int>(*dataByte)&0b1111;
//             if((static_cast<unsigned int>(*dataByte)&0b11110000)!= 0b10000){
//               firstEvent = true;
//               firstHead = true;
//               packetType = 0;
//               rawEvent.reset();
//               cout<<"ADC data error"<<endl;
//               return false;
//             }
//           }else{
//             channel.waveform[waveformFillStatusCode/2] = (channel.waveform[waveformFillStatusCode/2]<<8) | (static_cast<unsigned int>(*dataByte)&0b11111111);
//           }
//           waveformFillStatusCode++;
//           if(waveformFillStatusCode == 2048){
//             channel.event_id = _event_id;
//             channel.timestamp = _timestamp;
//             rawEvent.Fill(channel);
//             packetPose++;
//             waveformFillStatusCode = 0;
//           }
//           break;
//         case 8://reserved[15:8]
//           break;
//         case 9://reserved[7:0]
//           break;
//         case 10://CRC-32[31-24]
//           break;
//         case 11://CRC-32[23-16]
//           break;
//         case 12://CRC-32[15-8]
//           break;
//         case 13://CRC-32[7-0]
//           packetType = 0;
//           break;
//       }
//       break;
//     case 4:
//       packetPose++;
//       switch(packetPose){
//         case 3:
//           packetSize += static_cast<unsigned int>(*dataByte);
//           if(packetSize != 12)cout<<"end error"<<endl;
//           break;
//         case 4://FEE ID
//           break;
//         case 5://event size[31:24]
//           break;
//         case 6://event size[23:16]
//           break;
//         case 7://event size[15:8]
//           break;
//         case 8://event size[7:0]
//           break;
//         case 9://CRC-32[31-24]
//           break;
//         case 10://CRC-32[23-16]
//           break;
//         case 11://CRC-32[15-8] 
//           break;
//         case 12://CRC-32[7-0]
//           packetType = 0;
//           break;
//       }
//       break;
//   }
//   return false;
// }

bool PacketDecoder::Fill(const char* dataByte){
  //sleep(0.1);
  //cout<<"packetType: "<<packetType<< "packetPose: "<<packetPose<<" waveformFillStatusCode: "<<waveformFillStatusCode<<endl;
  switch(packetType){
    case 0:
      if(static_cast<unsigned int>(*dataByte) == 0x5a ) {
        packetPose = 1;
        packetType = 1;
      }
      break;
    case 1:
      packetSize = (static_cast<unsigned int>(*dataByte)&0b00011111)<<8;
      packetPose++;
      if((static_cast<unsigned int>(*dataByte) &0xE0) == 0x40){
        packetType = 2;
        break;
      }else if((static_cast<unsigned int>(*dataByte) &0xE0) == 0x0){
        packetType = 3;
        break;
      }else if((static_cast<unsigned int>(*dataByte) &0xE0) == 0x20){
        packetType = 4;
        break;
      }else{
        packetType = 0;
        break;
      }
    case 2:
      packetPose++;
      switch (packetPose){
        case 3:
          packetSize += static_cast<unsigned int>(*dataByte);
          if(packetSize != 20){
            cout<<"packet size error: head:"<<packetSize<<endl;
            firstEvent = true;
            firstPacket = true;
            packetType = 0;
            rawEvent.reset();
            return false;
          }
          break;
        case 4:
          FEE_ID = static_cast<unsigned int>(*dataByte)&0b00111111;
        case 5://timestamp 47-40 bits
          _timestamp = 0;
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 6://timestamp 39-32 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 7://timestamp 31-24 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 8://timestamp 23-16 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 9://timestamp 15-8 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 10://timestamp 7-0 bits
          _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 11://event id 31-24 bits
          _event_id = 0;
          _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 12://event id 23-16 bits
          _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 13://event id 15-8 bits
          _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          break;
        case 14://event id 7-0 bits
          _event_id = (_event_id<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
          if(firstPacket){
            firstPacket = false;
            rawEvent.event_id = _event_id;
            rawEvent.timestamp = _timestamp;
          }
          if(_event_id != rawEvent.event_id){
            firstPacket = true;
            if(firstEvent){
              firstEvent = false;
              rawEvent.reset();
              return false;
            }
            // cout<<"event id "<<rawEvent.event_id<<" channels "<<rawEvent.channels.size()<<endl;
            return true;
          }
          break;
        case 15://hit count
          if(firstPacket){
            firstPacket = false;
            rawEvent.event_id = _event_id;
            rawEvent.timestamp = _timestamp;
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
          packetSize += static_cast<unsigned int>(*dataByte);
          if(packetSize != 2060){
            cout<<"packetSize error, Body:"<<packetSize<<endl;
            firstEvent = true;
            firstPacket = true;
            packetType = 0;
            rawEvent.reset();
            return false;
          }
          break;
        case 4:
          channel.FEE_id = static_cast<unsigned int>(*dataByte)&0b00111111;
          break;
        case 5://channel index
          channel.channel_id = static_cast<unsigned int>(*dataByte)&0b00111111;
          break;
        case 6:
          break;
        case 7:
          packetPose--;
          if(waveformFillStatusCode%2 == 0){
            channel.waveform[waveformFillStatusCode/2] = static_cast<unsigned int>(*dataByte)&0b1111;
            if((static_cast<unsigned int>(*dataByte)&0b11110000)!= 0b10000){
              firstEvent = true;
              firstPacket = true;
              packetType = 0;
              rawEvent.reset();
              cout<<"ADC data error"<<endl;
              return false;
            }
          }else{
            channel.waveform[waveformFillStatusCode/2] = (channel.waveform[waveformFillStatusCode/2]<<8) | (static_cast<unsigned int>(*dataByte)&0b11111111);
          }
          waveformFillStatusCode++;
          if(waveformFillStatusCode == 2048){
            channel.event_id = _event_id;
            channel.timestamp = _timestamp;
            rawEvent.Fill(channel);
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
          packetSize += static_cast<unsigned int>(*dataByte);
          if(packetSize != 12){
            cout<<"packet size error, End:"<<packetSize<<endl;
            firstEvent = true;
            firstPacket = true;
            packetType = 0;
            rawEvent.reset();
            return false;
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
  return false;
}
