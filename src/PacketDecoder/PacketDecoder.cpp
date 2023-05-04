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

// bool PacketDecoder::Fill(const char* dataByte){
//   sleep(0.1);
//   // cout<<"findpacket: "<<findpacket<<" findwhat: "<<findwhat<<" fillStatusCode: "<<fillStatusCode<<endl;
//   switch(findpacket){
//     case 0://find event head
//       if(static_cast<unsigned int>(*dataByte) == 0x5a ) {findpacket = 1;findwhat=0b11111111;}
//       // cout<<"-----------------------------------1"<<endl;
//       break;

//     case 1:
//       if(findwhat!=0x40 && findwhat!=0x20 && findwhat!=0x00){
//         findwhat = static_cast<unsigned int>(*dataByte)&0xE0;
//         packetSize = (static_cast<unsigned int>(*dataByte)&0b00011111)<<8;
//         fillStatusCode = 0;
//         // cout<<"-----------------------------------2--findwhat"<<findwhat<<endl;
//         break;
//       }
//       switch (findwhat){
//         case 0x40:
//           switch (fillStatusCode)
//           {
//             case 0:
//               // cout<<"-----------------------------------3"<<endl;
//               fillStatusCode = 1;//packet size 0-7 bits
//               packetSize += static_cast<unsigned int>(*dataByte);
//               // cout<<"head: "<<packetSize<<endl;
//               cout<<"find head"<<endl;
//               if(packetSize!=20){cout<<"head: "<<packetSize<<endl;findpacket=0;findwhat=0b11111111;fillStatusCode = 0;break;}
//               break;
//             case 1:
//               // cout<<"-----------------------------------4"<<endl;
//               fillStatusCode = 2;//FEE ID
//               FEE_ID = static_cast<unsigned int>(*dataByte)&0b00111111;
//               if(FEE_ID<0 || FEE_ID>31){findpacket=0;findwhat=0b11111111;fillStatusCode = 0;}
//               // rawEvent.reset();
//               break;
//             case 2:
//               // cout<<"-----------------------------------5"<<endl;
//               _timestamp = 0;
//               fillStatusCode = 3;//timestamp 47-40 bits
//               _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//               break;
//             case 3:
//               // cout<<"-----------------------------------6"<<endl;
//               fillStatusCode = 4;//timestamp 39-32 bits
//               _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//               break;
//             case 4:
//               // cout<<"-----------------------------------7"<<endl;
//               fillStatusCode = 5;//timestamp 31-24 bits
//               _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//               break;
//             case 5:
//               // cout<<"-----------------------------------8"<<endl;
//               fillStatusCode = 6;//timestamp 23-16 bits
//               _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//               break;
//             case 6:
//               // cout<<"-----------------------------------9"<<endl;
//               fillStatusCode = 7;//timestamp 15-8 bits
//               _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//               break;
//             case 7:
//               // cout<<"-----------------------------------10"<<endl;
//               fillStatusCode = 8;//timestamp 7-0 bits
//               _timestamp = (_timestamp<<8) |(static_cast<uint64_t>(*dataByte) & 0xff);
//               // cout<<"timestamp: "<<timestamp<<endl;
//               break;
//             case 8:
//               // cout<<"-----------------------------------11"<<endl;
//               fillStatusCode = 9;//event id 31-24 bits
//               _event_id = 0;
//               _event_id = (_event_id<<8) |(static_cast<unsigned int>(*dataByte) & 0xff);
//               break;
//             case 9:
//               // cout<<"-----------------------------------12"<<endl;
//               fillStatusCode = 10;//event id 23-16 bits
//               _event_id = (_event_id<<8) |(static_cast<unsigned int>(*dataByte) & 0xff);
//               break;
//             case 10:
//               // cout<<"-----------------------------------13"<<endl;
//               fillStatusCode = 11;//event id 15-8 bits
//               _event_id = (_event_id<<8) |(static_cast<unsigned int>(*dataByte) & 0xff);
//               break;
//             case 11:
//               fillStatusCode = 12;//event id 7-0 bits
//               _event_id = (_event_id<<8) |(static_cast<unsigned int>(*dataByte) & 0xff);
//               if(_event_id<0)cout<<"_event_id: "<<_event_id<<endl;
//               if(firstPacket){
//                 firstPacket = false;
//                 rawEvent.event_id = _event_id;
//                 rawEvent.timestamp = _timestamp;
//                 temp_event_id = _event_id;
//                 temp_timestamp = _timestamp;
//               }
//               else{
//                 if(temp_event_id != _event_id){
//                   rawEvent.event_id = temp_event_id;
//                   rawEvent.timestamp = temp_timestamp;
//                   temp_event_id = _event_id;
//                   temp_timestamp = _timestamp;
//                   return true;
//                 }
//               }
//               break;
//             case 12:
//               fillStatusCode = 13;// hit count
//               rawEvent.hit_count = static_cast<unsigned int>(*dataByte)&0b00111111;
//               break;
//             case 13:
//               fillStatusCode = 14;//reserved
//               break;
//             case 14:
//               fillStatusCode = 15;//CRC-32[31-24]
//               break;
//             case 15:
//               fillStatusCode = 16;//CRC-32[23-16]
//               break;
//             case 16:
//               fillStatusCode = 17;//CRC-32[15-8]
//               break;
//             case 17:
//               fillStatusCode = 0;//CRC-32[7-0]
//               findpacket=0;
//               findwhat=0b11111111;
//               break;
              
//             default:
//               fillStatusCode = 0;
//               break;
//           }
//           break;

//         case 0x20:
//           switch (fillStatusCode){
//             case 0:
//               fillStatusCode = 1;//packet size 0-7 bits
//               packetSize += static_cast<unsigned int>(*dataByte);
//               // cout<<"end: "<<packetSize<<endl;
//               cout<<"find end"<<endl;
//               if(packetSize!=12){
//                 // cout<<"end: "<<packetSize<<endl;
//                 findpacket=0;findwhat=0b11111111;fillStatusCode = 0;break;}
//               break;
//             case 1:
//               fillStatusCode = 2;//FEE ID
//               break;
//             case 2:
//               fillStatusCode = 3;//event size[31-24]
//               break;
//             case 3:
//               fillStatusCode = 4;//event size[23-16]
//               break;
//             case 4:
//               fillStatusCode = 5;//event size[15-8]
//               break;
//             case 5:
//               fillStatusCode = 6;//event size[7-0]
//               break;
//             case 6:
//               fillStatusCode = 7;//CRC-32[31-24]
//               break;
//             case 7:
//               fillStatusCode = 8;//CRC-32[23-16]
//               break;
//             case 8:
//               fillStatusCode = 9;//CRC-32[15-8]
//               break;
//             case 9: 
//               fillStatusCode = 0;//CRC-32[7-0]
//               findpacket=0;
//               findwhat=0b11111111;
//               break;

//             default:
//               break;
//           }
//           break;

//         case 0x00:
//           switch (fillStatusCode){
//             case 0:
//               fillStatusCode = 1;//packet size 0-7 bits
//               packetSize += static_cast<unsigned int>(*dataByte);
//               // cout<<"body: "<<packetSize<<endl;
//               cout<<"find body"<<endl;
//               if(packetSize!=2060){
//                 // cout<<"body: "<<packetSize<<endl; 
//                 findpacket=0;findwhat=0b11111111;fillStatusCode = 0;break;}
//               break;
//             case 1:
//               fillStatusCode = 2;//FEE ID
//               channel.FEE_id = 0;
//               channel.FEE_id = static_cast<unsigned int>(*dataByte)&0b00111111;
//               if(channel.FEE_id<0 || channel.FEE_id>31){
//                 // cout<<"channel.FEE_id: "<<channel.FEE_id<<endl;
//                 findpacket=0;findwhat=0b11111111;fillStatusCode = 0;}
//               break;
//             case 2:
//               fillStatusCode = 3;//channel id
//               channel.channel_id = static_cast<unsigned int>(*dataByte)&0b00111111;
//               // if(channel.channel_id>63||channel.channel_id<0)cout<<"channel.channel_id: "<<channel.channel_id<<endl;
//               break;
//             case 3:
//               fillStatusCode = 4;//reserved
//               break;
//             case 4:
//               fillStatusCode = 5;//ADC data 0[11:8]
//               // if((static_cast<unsigned int>(*dataByte)&0xF0) != 0b00010000)fillStatusCode = 0;
//               channel.waveform[waveformFillStatusCode] = 0;
//               channel.waveform[waveformFillStatusCode] = static_cast<unsigned int>(*dataByte)&0b00001111;
//               break;
//             case 5:
//               fillStatusCode = 4;//ADC data 0[7:0]
//               channel.waveform[waveformFillStatusCode] = (channel.waveform[waveformFillStatusCode]<<8) | (static_cast<unsigned int>(*dataByte)&0b11111111);
//               // if(waveform[waveformFillStatusCode]>4096||waveform[waveformFillStatusCode]<560)cout<<"waveform[waveformFillStatusCode]: "<<waveform[waveformFillStatusCode]<<endl;
//               waveformFillStatusCode++;
//               if(waveformFillStatusCode == 1024){
//                 waveformFillStatusCode = 0;
//                 fillStatusCode = 6;
//                 channel.event_id = temp_event_id;
//                 channel.timestamp = temp_timestamp;
//                 rawEvent.Fill(channel);
//                 } 
//               break;
//             case 6:
//               fillStatusCode = 7;//reserved[15:8]
//               break;
//             case 7:
//               fillStatusCode = 8;//reserved[7:0]
//               break;
//             case 8:
//               fillStatusCode = 9;//CRC-32[31-24]
//               break;
//             case 9:
//               fillStatusCode = 10;//CRC-32[23-16]
//               break;
//             case 10:
//               fillStatusCode = 11;//CRC-32[15-8]
//               break;
//             case 11: 
//               fillStatusCode = 0;//CRC-32[7-0]
//               findpacket=0;
//               findwhat=0b11111111;
//               break;

//             default:
//               break;
//           }
//           break;
//         default:
//           findwhat = 0b11111111;
//           break;
//       }
//       break;

//     default:
//       findpacket=0;
//       break;
//   }
//   return false;
// }

// bool PacketDecoder::Fill(const char* dataByte){
//     switch(fillStatusCode){
//       case 0://find event head
//         if(*dataByte == 0x5a ) fillStatusCode = 1;
//         break;

//       case 1:
//         if((*dataByte&0b01100000) == 0b01000000 ){
//           fillStatusCode = 2;//event head found
//           packetSize = uint((*dataByte&0b00011111)<<8);
//           rawEvent.reset();
//         }else{
//           fillStatusCode = 0;//event head not found
//         }
//         break;

//       case 2:
//         fillStatusCode = 3;//packet size 0-7 bits
//         packetSize += uint(*dataByte);
//         if(packetSize != 20)fillStatusCode = 0;
//         break;

//       case 3:
//         fillStatusCode = 4;//FEE ID
//         FEE_ID = uint(*dataByte&0b00111111);
//         break;

//       case 4:
//         fillStatusCode = 5;//timestamp 47-40 b check if the timestamp is correct
//         rawEvent.timestamp = static_cast<Long64_t>(*dataByte) << 40;
//         break;

//       case 5:
//         fillStatusCode = 6;//timestamp 39-32 b check if the timestamp is correct
//         rawEvent.timestamp |= static_cast<Long64_t>(*dataByte) << 32;
//         break;

//       case 6:
//         fillStatusCode = 7;//timestamp 31-24 bits
//         rawEvent.timestamp |= static_cast<Long64_t>(*dataByte) << 24;
//         break;

//       case 7:
//         fillStatusCode = 8;//timestamp 23-16 bits
//         rawEvent.timestamp |= static_cast<Long64_t>(*dataByte) << 16;
//         break;

//       case 8:
//         fillStatusCode = 9;//timestamp 15-8 bits
//         rawEvent.timestamp |= static_cast<Long64_t>(*dataByte) << 8;
//         break;

//       case 9:
//         fillStatusCode = 10;//timestamp 7-0 bits
//         rawEvent.timestamp |= static_cast<Long64_t>(*dataByte);
//         break;

//       case 10:
//         fillStatusCode = 11;//event id 31-24 b check if the event id is correct
//         rawEvent.event_id = static_cast<Long64_t>(*dataByte) << 24;
//         break;

//       case 11:
//         fillStatusCode = 12;//event id 23-16 bits
//         rawEvent.event_id |= static_cast<Long64_t>(*dataByte) << 16;
//         break;

//       case 12:
//         fillStatusCode = 13;//event id 15-8 bits
//         rawEvent.event_id |= static_cast<Long64_t>(*dataByte) << 8;
//         break;

//       case 13:
//         fillStatusCode = 14;//event id 7-0 bits
//         rawEvent.event_id |= static_cast<Long64_t>(*dataByte);
//         break;

//       case 14:
//         fillStatusCode = 15;// hit count
//         rawEvent.hit_count = uint(*dataByte&0b00111111);
//         if(rawEvent.hit_count != 0)fillStatusCode = 0;
//         break;
//       //-----
//       case 15:
//         fillStatusCode = 16;//reserved
//         break;
//       case 16:
//         fillStatusCode = 17;//CRC-32[31-24]
//         break;
//       case 17:
//         fillStatusCode = 18;//CRC-32[23-16]
//         break;
//       case 18:
//         fillStatusCode = 19;//CRC-32[15-8]
//         break;
//       case 19:
//         fillStatusCode = 20;//CRC-32[7-0]
//         break;


//       case 20://find channel head or event tail
//         if(*dataByte == 0x5a ) fillStatusCode = 21;//find channel head
//         break;

//       case 21:
//         if((*dataByte&0b01100000) == 0b00000000 ){
//           fillStatusCode = 22;//channel head found
//           packetSize = uint((*dataByte&0b00011111)<<8);
//         }else if((*dataByte&0b01100000) == 0b00100000 ){
//           fillStatusCode = 34;//event tail found
//           return true;
//         }else{
//           fillStatusCode = 20;//channel head not found
//         }
//         break;

//       case 22:
//         fillStatusCode = 23;//packet size 0-7 bits
//         packetSize += uint(*dataByte);
//         break;

//       case 23://这里识别有错误会产生过大的值，会导致基于FEE-id的计算出错
//         fillStatusCode = 24;//FEE ID
//         channel.FEE_id = uint(*dataByte&0b00111111);
//         break;

//       case 24:
//         fillStatusCode = 25;//channel id
//         channel.channel_id = uint(*dataByte&0b00111111);
//         break;

//       case 25:
//         fillStatusCode = 26;//reserved
//         break;

//       case 26:
//         fillStatusCode = 27;//ADC data 0[11:8]
//         channel.waveform[waveformFillStatusCode] = UInt_t((*dataByte&0b1111)<<8);
//         break;

//       case 27:
//         fillStatusCode = 26;//ADC data 0[7:0]
//         channel.waveform[waveformFillStatusCode] += UInt_t(*dataByte);
//         waveformFillStatusCode++;
//         if(waveformFillStatusCode == 1024){
//           waveformFillStatusCode = 0;
//           fillStatusCode = 28;
//           channel.event_id = rawEvent.event_id;
//           rawEvent.Fill(channel);
//           } 
//         break;
//       case 28:
//         fillStatusCode = 29;//reserved[15:8]
//         break;
//       case 29:
//         fillStatusCode = 30;//reserved[7:0]
//         break;
//       case 30:
//         fillStatusCode = 31;//CRC-32[31-24]
//         break;
//       case 31:
//         fillStatusCode = 32;//CRC-32[23-16]
//         break;
//       case 32:
//         fillStatusCode = 33;//CRC-32[15-8]
//         break;
//       case 33: 
//         fillStatusCode = 20;//CRC-32[7-0]
//         break;

//       case 34:
//         fillStatusCode = 35;//packet size[12
//         break;
//       case 35:
//         fillStatusCode = 36;//packet size[7
//         break;
//       case 36:
//         fillStatusCode = 37;//FEE
//         break;
//       case 37:
//         fillStatusCode = 38;//event size[31-
//         break;
//       case 38:
//         fillStatusCode = 39;//event size[23-
//         break;
//       case 39:
//         fillStatusCode = 40;//event size[15
//         break;
//       case 40:
//         fillStatusCode = 41;//event size[7
//         break;
//       case 41:
//         fillStatusCode = 42;//CRC-32[31-24]
//         break;
//       case 42:
//         fillStatusCode = 43;//CRC-32[23-16]
//         break;
//       case 43:
//         fillStatusCode = 44;//CRC-32[15-8]
//         break;
//       case 44: 
//         fillStatusCode = 0;//CRC-32[7-0]
//         break;
      
//       default:
//         break;
//     }
//   return false;
// }