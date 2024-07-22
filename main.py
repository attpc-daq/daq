# -*- coding:utf-8 -*-
import argparse
import GUISocket
import ROOT
import sys
import time
import asyncio
import os
import json
import numpy as np
import ast
import socket
import psutil

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "AutoSocket.h"')
ROOT.gInterpreter.Declare('#include "DataProcessor.h"')
ROOT.gInterpreter.Declare('#include "Event.h"')
ROOT.gInterpreter.Declare('#include "EventConverter.h"')
ROOT.gInterpreter.Declare('#include "EventQA.h"')
ROOT.gInterpreter.Declare('#include "PacketDecoder.h"')
ROOT.gInterpreter.Declare('#include "ParameterGenerator.h"')
ROOT.gInterpreter.Declare('#include "RawEvent.h"')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gInterpreter.Declare('#include "TMessageBuffer.h"')
ROOT.gInterpreter.Declare('#include "TMessageBufferTP.h"')
ROOT.gInterpreter.Declare('#include "TMessageSocket.h"')

ROOT.gSystem.Load("libAutoSocket.so")
ROOT.gSystem.Load("libDataProcessor.so")
ROOT.gSystem.Load("libEvent.so")
ROOT.gSystem.Load("libEventConverter.so")
ROOT.gSystem.Load("libEventQA.so")
ROOT.gSystem.Load("libPacketDecoder.so")
ROOT.gSystem.Load("libParameterGenerator.so")
ROOT.gSystem.Load("libRawEvent.so")
ROOT.gSystem.Load("libSiTCP.so")
ROOT.gSystem.Load("libTMessageBuffer.so")
ROOT.gSystem.Load("libTMessageSocket.so")
ROOT.gSystem.Load("libHist.so")


class DAQHandler(GUISocket.Utils.WebsocketHander):
    '''
    websocket handler 响应GUI命令
    '''
    def __init__(self):
        super(DAQHandler, self).__init__()
        self.sitcp = [None,None]
        self.dataProcessor = None
        self.eventQA = None
        self.logger = None
        self.outputDir = './output/'
        #update: by whk
        # self.Wvalue = 30.0 #unit: eV
        # self.Vdrift = 10.0 #unit: mm/ns
        # self.fpc2_vector = ROOT.vector(ROOT.map('string', 'int'))()

        # self.current_process_id = psutil.Process()
        # print("Current Process ID:", self.current_process_id.pid)
        
    #—————————————————————————————————————————————————————————————
    #自定义函数应为协程函数，前缀为'on_cmd_'，参数列表不可变更
    #—————————————————————————————————————————————————————————————
    def convert(self, addr, value):
        return '0'+str(addr[3])+'1'+str(addr[2])+'2'+str(addr[1])+'3'+str(addr[0])+'4'+str(value[3])+'5'+str(value[2])+'6'+str(value[1])+'7'+str(value[0])+'83'
    
    async def on_cmd_send_to_SiTCP1(self, websocket, msg_list, client_key):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self.sitcp[0].getIP(), self.sitcp[0].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            print("send to SiTCP1 Caught exception socket.error : %s" % exc)
            self.log("send to SiTCP1 Caught exception socket.error : %s" % exc)
            return
        msg = bytes.fromhex(str(msg_list[1]))
        sock.send(msg)
        message = "send to SiTCP1 "+str(msg_list[1]) + " done"
        self.log(message)
        # await websocket.send(message)
        sock.close()
    
    async def on_cmd_send_to_SiTCP2(self, websocket, msg_list, client_key):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self.sitcp[1].getIP(), self.sitcp[1].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            print("send to SiTCP2 Caught exception socket.error : %s" % exc)
            self.log("send to SiTCP1 Caught exception socket.error : %s" % exc)
        
            return
        msg = bytes.fromhex(str(msg_list[1]))
        sock.send(msg)
        message =  "send to SiTCP2 "+str(msg_list[1]) + " done"
        self.log(message)
        # await websocket.send(message)
        sock.close()
        
    # async def on_cmd_send_cmd(self, websocket, cmd_list, client_key):
    #     if(cmd_list[1] == 'SiTCP1'):
    #         await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert(cmd_list[2],cmd_list[3])], client_key)
    #         time.sleep(1)
    #     if(cmd_list[1] == 'SiTCP2'):
    #         await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert(cmd_list[2],cmd_list[3])], client_key)
    #         time.sleep(1)

    # async def on_cmd_dumpSitcp(self, websocket, cmd_list, client_key):
    #     if self.sitcp[0] is not None:
    #         await websocket.send("dataRate1 "+str(self.sitcp[0].getRate()))
    #         await websocket.send("nTask1 "+str(self.sitcp[0].getNTasks()))
    #         await websocket.send("decoderState1 "+str(self.sitcp[0].getDecoderState()))
    #         await websocket.send("daqState1 "+str(self.sitcp[0].getState()))
    #     else:
    #         await websocket.send("nositcp1")

    #     if self.sitcp[1] is not None:
    #         await websocket.send("dataRate2 "+str(self.sitcp[1].getRate()))
    #         await websocket.send("nTask2 "+str(self.sitcp[1].getNTasks()))
    #         await websocket.send("decoderState2 "+str(self.sitcp[1].getDecoderState()))
    #         await websocket.send("daqState2 "+str(self.sitcp[1].getState()))
    #     else:
    #         await websocket.send("nositcp2")

    #     process = psutil.Process()
    #     python_cpu_percent = process.cpu_percent(interval=1.0)
    #     python_memory_info = process.memory_info()

    #     print("Python Process CPU Usage: {}%".format(python_cpu_percent))
    #     print("Python Process Memory Usage: {} Mbytes".format((python_memory_info.rss)/1000000))

    async def on_cmd_initSitcp(self, websocket, cmd_list, client_key):
        '''
        初始化SiTCP
        '''
        if self.sitcp[0] is None:
            self.sitcp[0] = ROOT.SiTCP(1)
            self.sitcp[0].resetSHM()
            self.sitcp[0].setServerAddress(cmd_list[1], int(cmd_list[2]))
            self.sitcp[0].setDir(cmd_list[3])
            self.sitcp[0].setDataPort(8010)
            self.sitcp[0].setFileMaxSize(int(cmd_list[7]))
            self.sitcp[0].disableBuffer()

        if self.sitcp[1] is None:
            self.sitcp[1] = ROOT.SiTCP(2)
            self.sitcp[1].resetSHM()
            self.sitcp[1].setServerAddress(cmd_list[4], int(cmd_list[5]))
            self.sitcp[1].setDir(cmd_list[6])
            self.sitcp[1].setDataPort(8011)
            self.sitcp[1].setFileMaxSize(int(cmd_list[7]))
            self.sitcp[1].disableBuffer()

    async def on_cmd_setSiTCP1BufferFilePath(self, websocket, cmd_list, client_key):
        '''
        设置SiTCP1缓存文件路径
        '''
        if self.sitcp[0] is not None:
            self.sitcp[0].setDir(cmd_list[1])

    async def on_cmd_setSiTCP2BufferFilePath(self, websocket, cmd_list, client_key):
        '''
        设置SiTCP2缓存文件路径
        '''
        if self.sitcp[1] is not None:
            self.sitcp[1].setDir(cmd_list[1])

    async def on_cmd_getSiTCPState(self, websocket, cmd_list, client_key):
            rsp = "SiTCPState"
            if self.sitcp[0] is None:
                rsp += " -1"
            else:
                rsp += " "+str(self.sitcp[0].getState())
            if self.sitcp[1] is None:
                rsp += " -1"
            else:
                rsp += " "+str(self.sitcp[1].getState())
            await websocket.send(rsp)

    async def on_cmd_getSiTCPDataAcquisitionState(self, websocket, cmd_list, client_key):
            rsp = "SiTCPDataAcquisitionState"
            if self.sitcp[0] is None:
                rsp += " -1"
            else:
                rsp += " "+str(self.sitcp[0].getDataAcquisitionState())
            if self.sitcp[1] is None:
                rsp += " -1"
            else:
                rsp += " "+str(self.sitcp[1].getDataAcquisitionState())
            await websocket.send(rsp)
    
    async def on_cmd_getSiTCPDecoderState(self, websocket, cmd_list, client_key):
            rsp = "SiTCPDecoderState"
            if self.sitcp[0] is None:
                rsp += " -1"
            else:
                rsp += " "+str(self.sitcp[0].getDecoderState())
            if self.sitcp[1] is None:
                rsp += " -1"
            else:
                rsp += " "+str(self.sitcp[1].getDecoderState())
            await websocket.send(rsp)
    
    async def on_cmd_shutdownSiTCP(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            del (self.sitcp[0])
        if self.sitcp[0] is not None:
            del (self.sitcp[0])
        
        self.sitcp = [None, None]

    async def on_cmd_startSiTCP(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].start()
        if self.sitcp[1] is not None:
            self.sitcp[1].start()
    
    async def on_cmd_stopSiTCP(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].stop()
        if self.sitcp[1] is not None:
            self.sitcp[1].stop()

    async def on_cmd_startSiTCPData(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('a000','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0410203A4150607083'], client_key)
        time.sleep(2)
        if self.sitcp[0] is not None:
            self.sitcp[0].startData()
        if self.sitcp[1] is not None:
            self.sitcp[1].startData()

    async def on_cmd_stopSiTCPData(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].stopData()
        if self.sitcp[1] is not None:
            self.sitcp[1].stopData()
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0410203A4050607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '081020384150607083'], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '081020384150607083'], client_key)
        time.sleep(1)

    async def on_cmd_startSiTCPDecoder(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].startDecoder()
        if self.sitcp[1] is not None:
            self.sitcp[1].startDecoder()
    
    async def on_cmd_stopSiTCPDecoder(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].stopDecoder()
        if self.sitcp[1] is not None:
            self.sitcp[1].stopDecoder()
    
    async def on_cmd_getSiTCP1DataRate(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            await websocket.send("dataRate1 "+str(self.sitcp[0].getRate()))
        else:
            await websocket.send("dataRate1 -1")
    
    async def on_cmd_getSiTCP2DataRate(self, websocket, cmd_list, client_key):
        if self.sitcp[1] is not None:
            await websocket.send("dataRate2 "+str(self.sitcp[1].getRate()))
        else:
            await websocket.send("dataRate2 -1")

    async def on_cmd_getSiTCP1NTask(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            await websocket.send("nTask1 "+str(self.sitcp[0].getNTasks()))
        else:
            await websocket.send("nTask1 -1")
        
    async def on_cmd_getSiTCP2NTask(self, websocket, cmd_list, client_key):
        if self.sitcp[1] is not None:
            await websocket.send("nTask2 "+str(self.sitcp[1].getNTasks()))
        else:
            await websocket.send("nTask2 -1")

    async def on_cmd_setBufferFileSize(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].setFileMaxSize(int(cmd_list[1]))
        if self.sitcp[1] is not None:
            self.sitcp[1].setFileMaxSize(int(cmd_list[1]))

        await websocket.send("setBufferFileSize done!")
    
    async def on_cmd_setupthreshold(self, websocket, cmd_list, client_key):

        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('800a','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('a006','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('800a','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('a006','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('1004','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('0000','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('8004','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('1004','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('0000','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('8004','0001')], client_key)
        time.sleep(0.1)

        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('1800','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('1812','0001')], client_key)
        time.sleep(0.1)
        
        with open(cmd_list[1],'r') as file:
            content = file.read()
            msgJson = json.loads(content)
        
        # GUISocket.Utils.LOGGER.info("SiTCP threshold setting:")
        self.log("SiTCP threshold setting:")

        sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock1.settimeout(3.0)
        try:
            sock1.connect((self.sitcp[0].getIP(), self.sitcp[0].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            print("Caught SiTCP1 exception socket.error : %s" % exc)
            return

        for i in range(msgJson['count']):
            for key in msgJson[str(i)]:
                for k, v in key.items():
                    if k=='waveform_mean' or k=='waveform_rms':
                        continue
                    cmd = bytes.fromhex(v)
                    try:
                        sock1.send(cmd)
                        time.sleep(0.001)
                        # GUISocket.Utils.LOGGER.info(f"{k}: {v}")
                        self.log(f"{k}: {v}")
                    except:
                        print("SiTCP threshold setting error!")
            if i%100 == 0:
                await websocket.send("thresholdsetting "+str(int(i/20.48/4))+"%")
        #设置束流空间触发阈值为0fff，束流空间不参与触发
        # for i in range(msgJson['BeamSpace']):
        #     for key in msgJson['Beam'+str(i)]:
        #         for k, v in key.items():
        #             cmd = bytes.fromhex(v)
        #             try:
        #                 sock1.send(cmd)
        #                 time.sleep(0.001)
        #                 # GUISocket.Utils.LOGGER.info(f"{k}: {v}")
        #                 self.log(f"BeamSpace {k}: {v}")
        #             except:
        #                 print("SiTCP threshold setting error!")
        sock1.close()

        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('1812','0000')], client_key)
        time.sleep(0.1)

        sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock1.settimeout(3.0)
        try:
            sock1.connect((self.sitcp[0].getIP(), self.sitcp[0].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            print("Caught SiTCP1 exception socket.error : %s" % exc)
            return

        for i in range(msgJson['count']):
            for key in msgJson[str(i)]:
                for k, v in key.items():
                    if k=='waveform_mean' or k=='waveform_rms':
                        continue
                    cmd = bytes.fromhex(v)
                    try:
                        sock1.send(cmd)
                        time.sleep(0.001)
                        # GUISocket.Utils.LOGGER.info(f"{k}: {v}")
                        self.log(f"{k}: {v}")
                    except:
                        print("SiTCP threshold setting error!")
            if i%100 == 0:
                await websocket.send("thresholdsetting "+str(int((i+2048)/20.48/4))+"%")
        sock1.close()
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('1800','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('1812','0001')], client_key)
        time.sleep(0.1)

        sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock2.settimeout(3.0)
        try:
            sock2.connect((self.sitcp[1].getIP(), self.sitcp[1].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            message = "Can not connect to SiTCP2 port"
            print("Caught SiTCP2 exception socket.error : %s" % exc)
            await websocket.send(message)
            return

        for i in range(msgJson['count']):
            for key in msgJson[str(i)]:
                for k, v in key.items():
                    if k=='waveform_mean' or k=='waveform_rms':
                        continue
                    cmd = bytes.fromhex(v)
                    try:
                        sock2.send(cmd)
                        time.sleep(0.001)
                        # GUISocket.Utils.LOGGER.info(f"{k}: {v}")
                        self.log(f"{k}: {v}")
                    except:
                        print("SiTCP threshold setting error!")
            if i%100 == 0:
                await websocket.send("thresholdsetting "+str(int((i+2048*2)/20.48/4))+"%")
        #设置束流空间触发阈值为0fff，束流空间不参与触发
        # for i in range(msgJson['BeamSpace']):
        #     for key in msgJson['Beam'+str(i)]:
        #         for k, v in key.items():
        #             cmd = bytes.fromhex(v)
        #             try:
        #                 sock2.send(cmd)
        #                 time.sleep(0.001)
        #                 # GUISocket.Utils.LOGGER.info(f"{k}: {v}")
        #                 self.log(f"BeamSpace {k}: {v}")
        #             except:
        #                 print("SiTCP threshold setting error!")
        sock2.close()
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('1812','0000')], client_key)
        time.sleep(0.1)

        sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock2.settimeout(3.0)
        try:
            sock2.connect((self.sitcp[1].getIP(), self.sitcp[1].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            message = "Can not connect to SiTCP2 port"
            print("Caught SiTCP2 exception socket.error : %s" % exc)
            await websocket.send(message)
            return

        for i in range(msgJson['count']):
            for key in msgJson[str(i)]:
                for k, v in key.items():
                    if k=='waveform_mean' or k=='waveform_rms':
                        continue
                    cmd = bytes.fromhex(v)
                    try:
                        sock2.send(cmd)
                        time.sleep(0.001)
                        # GUISocket.Utils.LOGGER.info(f"{k}: {v}")
                        self.log(f"{k}: {v}")
                    except:
                        print("SiTCP threshold setting error!")
            if i%100 == 0:
                await websocket.send("thresholdsetting "+str(int((i+2048*3)/20.48/4))+"%")
        sock2.close()
        await websocket.send("thresholdsetting done!")

    async def on_cmd_selftrigger(self, websocket, cmd_list, client_key):
        # send_to_SiTCP1  192.168.10.16
        # send_to_SiTCP2  192.168.10.15
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('800a','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('a006','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('800a','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('a006','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('1004','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('0000','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('8004','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('1004','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('0000','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('8004','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('1800','0000')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('1814','2d00')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('1816','0131')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('1800','0000')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('1814','2d00')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('1816','0131')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('8008','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('9002','ffff')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('8008','0001')], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', self.convert('9002','ffff')], client_key)
        time.sleep(1)
        # await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', self.convert('a000','0001')], client_key)
        # time.sleep(1)
        await websocket.send("selftrigger done!")
      
    async def on_cmd_set_T0(self, websocket, cmd_list, client_key):
        startTime = int(cmd_list[3])
        startTime = int(startTime/8.33)
        if(startTime <= 0):
            startTime = 1
        startTime_hex = hex(startTime)
        startTime_str = str(startTime_hex)[2:].zfill(8)
        stopTime = int(cmd_list[4])
        stopTime = int(stopTime/8.33)
        stopTime_hex = hex(stopTime)
        stopTime_str = str(stopTime_hex)[2:].zfill(8)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B000','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B002',startTime_str[-4:])], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B004',startTime_str[:-4])], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B006',stopTime_str[-4:])], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B008',stopTime_str[:-4])], client_key)
        time.sleep(0.1)
        
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B000','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B002',startTime_str[-4:])], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B004',startTime_str[:-4])], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B006',stopTime_str[-4:])], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B008',stopTime_str[:-4])], client_key)
        time.sleep(0.1)
        
        if(cmd_list[1] == 'external'):
            await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B010','0000')], client_key)
            time.sleep(0.1)
            await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B010','0000')], client_key)
            time.sleep(0.1)
        elif(cmd_list[1] == 'self-T0'):
            frequency = int(cmd_list[2])
            period = int(1E9/frequency)#ns
            ncall = int(period/8.33)
            #ncall转化为16进制
            ncall_hex = hex(ncall)
            #将ncall_hex转化为字符串,共8位
            ncall_str = str(ncall_hex)[2:].zfill(8)
            await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B010','0001')], client_key)
            time.sleep(0.1)
            await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B012',ncall_str[-4:])], client_key)
            time.sleep(0.1)
            await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('B014',ncall_str[:-4])], client_key)
            time.sleep(0.1)
            
            await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B010','0001')], client_key)
            time.sleep(0.1)
            await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B012',ncall_str[-4:])], client_key)
            time.sleep(0.1)
            await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('B014',ncall_str[:-4])], client_key)
            time.sleep(0.1)
        await websocket.send('T0 mode done!')

    async def on_cmd_setfeenchannel(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['', cmd_list[1]], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('8008','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP1(websocket,['', self.convert('9002','ffff')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', cmd_list[1]], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('8008','0001')], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['', self.convert('9002','ffff')], client_key)
        time.sleep(0.1)
        await websocket.send('setfeenchannel done!')

    async def on_cmd_setfeeslope(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', cmd_list[1]], client_key)
        time.sleep(0.1)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', cmd_list[1]], client_key)
        time.sleep(0.1)
        await websocket.send('setfeeslope done!')

    async def on_cmd_getDataProcessorState(self, websocket, cmd_list, client_key):
        rsp = "DataProcessorState"
        if self.dataProcessor is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.dataProcessor.getState())
        await websocket.send(rsp)

    async def on_cmd_initDataProcessor(self, websocket, cmd_list, client_key):
        self.dataProcessor = ROOT.DataProcessor()
        self.dataProcessor.resetSHM()
        self.dataProcessor.setDir(cmd_list[1])
        self.dataProcessor.setDataPort(8010,"localhost",8011,"localhost")
        self.dataProcessor.setEventSave(False)
        self.dataProcessor.setRawEventSave(False)
        self.dataProcessor.setFileEvents(int(cmd_list[2]))

    async def on_cmd_shutdownDataProcessor(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            del self.dataProcessor
            self.dataProcessor = None
    
    async def on_cmd_startDataProcessor(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            self.dataProcessor.start()
    
    async def on_cmd_stopDataProcessor(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            self.dataProcessor.stop()
    
    async def on_cmd_setDataProcessorEventsPerFile(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            self.dataProcessor.setFileEvents(int(cmd_list[1]))

    async def on_cmd_setDataProcessorDir(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            self.dataProcessor.setDir(cmd_list[1])

    async def on_cmd_getDataProcessorTotalEvent(self, websocket, cmd_list, client_key):
        rsp = "DataProcessorTotalEvent"
        if self.dataProcessor is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.dataProcessor.getTotalEvent())
        await websocket.send(rsp)
    
    async def on_cmd_getDataProcessorCurrentEventID(self, websocket, cmd_list, client_key):
        rsp = "DataProcessorCurrentEventID"
        if self.dataProcessor is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.dataProcessor.getCurrentEventID())
        await websocket.send(rsp)

    async def on_cmd_getDataProcessorCurrentFileID(self, websocket, cmd_list, client_key):
        rsp = "DataProcessorCurrentFileID"
        if self.dataProcessor is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.dataProcessor.getOutputFileID())
        await websocket.send(rsp)
    
    async def on_cmd_setDataProcessorParameterEvents(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            self.dataProcessor.generateParameters(int(cmd_list[1]))
    
    async def on_cmd_getDataProcessorParameterEvents(self, websocket, cmd_list, client_key):
        rsp = "DataProcessorParameterEvents"
        if self.dataProcessor is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.dataProcessor.getNMakePar())
        await websocket.send(rsp)
    
    async def on_cmd_turnOnRawEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setRawEventSave(True)

    async def on_cmd_turnOffRawEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setRawEventSave(False)

    async def on_cmd_turnOnEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setEventSave(True)

    async def on_cmd_turnOffEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setEventSave(False)

    #update: by whk
    async def on_cmd_setWvalueAndVdrift(self, websocket, cmd_list, client_key):
        self.dataProcessor.setWValue(float(cmd_list[1]))
        self.dataProcessor.setVdrift(float(cmd_list[2]))

    async def on_cmd_setGainFile(self, websocket, cmd_list, client_key):
        # print(cmd_list[1],cmd_list[2])
        self.dataProcessor.setElectronicFile(cmd_list[1])
        self.dataProcessor.setMicromegasFile(cmd_list[2])

    async def on_cmd_setFPC2(self, websocket, cmd_list, client_key):
        fpc2_vector = ROOT.vector(ROOT.map('string', 'int'))()
        fpc2_list = ast.literal_eval(str(cmd_list[1]))
        # 将Python列表的数据填充到ROOT的vector对象中
        for item in fpc2_list:
            map_obj = ROOT.map('string', 'int')()
            for key, value in item.items():
                map_obj[key] = int(value)
            fpc2_vector.push_back(map_obj)
            
        self.dataProcessor.setFPC2(fpc2_vector)
                
    async def on_cmd_getQAState(self, websocket, cmd_list, client_key):
        rsp = "QAState"
        if self.eventQA is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.eventQA.getState())
        await websocket.send(rsp)
    
    async def on_cmd_initQA(self, websocket, cmd_list, client_key):
        if self.eventQA is not None:
            return
        file = open(os.path.join(cmd_list[2],'./eventParameters.json'), 'r')
        settingJson = file.read()
        if file is None:
            print("Error: eventParameters.json for QA not found!")
            return
        self.eventQA = ROOT.EventQA()
        self.eventQA.setTHttpServerPort(int(cmd_list[1]))
        self.eventQA.setDir(cmd_list[2])
        self.eventQA.setpad_numQA(int(cmd_list[3]),int(cmd_list[4]))
        self.eventQA.updateSettings(settingJson)
    
    async def on_cmd_shutdownQA(self, websocket, cmd_list, client_key):
        if self.eventQA is not None:
            del self.eventQA
            self.eventQA = None

    async def on_cmd_startQA(self, websocket, cmd_list, client_key):
        self.eventQA.start()
    
    async def on_cmd_stopQA(self, websocket, cmd_list, client_key):
        self.eventQA.stop()
    
    async def on_cmd_getQATotalEvent(self, websocket, cmd_list, client_key):
        rsp = "QATotalEvent"
        if self.eventQA is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.eventQA.getTotalEvent())
        await websocket.send(rsp)
    
    async def on_cmd_getQACurrentEventID(self, websocket, cmd_list, client_key):
        rsp = "QACurrentEventID"
        if self.eventQA is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.eventQA.getCurrentEventID())
        await websocket.send(rsp)

    async def on_cmd_getQAList(self, websocket, cmd_list, client_key):
        await websocket.send("QAList "+self.eventQA.getList())

    async def on_cmd_getQA(self, websocket, cmd_list, client_key):
        if len(cmd_list)==1:
            return
        str = self.eventQA.get(cmd_list[1])
        await websocket.send(str)

    async def on_cmd_setQADir(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.setDir(cmd_list[1])
        
    async def on_cmd_setQAPad(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        print('setQAPad: ',cmd_list[1],cmd_list[2])
        self.eventQA.setpad_numQA(int(cmd_list[1]),int(cmd_list[2]))

    async def on_cmd_QAClearPlots(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.clearPlots()

    async def on_cmd_QADoFirstFile(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doFirstFile()

    async def on_cmd_QADoPreviousFile(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doPreviousFile()
    
    async def on_cmd_QADoFile(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doFile(int(cmd_list[1]))

    async def on_cmd_QADoNextFile(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doNextFile()

    async def on_cmd_QADoLastFile(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doLastFile()

    async def on_cmd_QASetAutoFile(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.setAutoFile()

    async def on_cmd_QADoFirstEvent(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doFirstEvent()

    async def on_cmd_QADoPreviousEvent(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doPreviousEvent()
    
    async def on_cmd_QADoEvent(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doEvent(int(cmd_list[1]))

    async def on_cmd_QADoNextEvent(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doNextEvent()

    async def on_cmd_QADoLastEvent(self, websocket, cmd_list, client_key):
        if self.eventQA is None:
            return
        self.eventQA.doLastEvent()

    # async def on_cmd_QASetAutoEvent(self, websocket, cmd_list, client_key):
    #     if self.eventQA is None:
    #         return
    #     self.eventQA.setAutoEvent()

    async def on_cmd_getQAAutoFileMode(self, websocket, cmd_list, client_key):
        rsp = "QAAutoFileMode"
        if self.eventQA is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.eventQA.getAutoFileMode())
        await websocket.send(rsp)
    
    # async def on_cmd_getQAAutoEventMode(self, websocket, cmd_list, client_key):
    #     rsp = "QAAutoEventMode"
    #     if self.eventQA is None:
    #         rsp += " -1"
    #     else:
    #         rsp += " " + str(self.eventQA.getAutoEventMode())
    #     await websocket.send(rsp)

    async def on_cmd_getQAFileID(self, websocket, cmd_list, client_key):
        rsp = "QAFileID"
        if self.eventQA is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.eventQA.getCurrentFileID())
        await websocket.send(rsp)

    async def on_cmd_getQAEventEntryID(self, websocket, cmd_list, client_key):
        rsp = "QAEventEntryID"
        if self.eventQA is None:
            rsp += " -1"
        else:
            rsp += " " + str(self.eventQA.getCurrentEventEntryID())
        await websocket.send(rsp)

    # async def on_cmd_shutdown(self, websocket, cmd_list, client_key):
    #     await websocket.close()
    #     asyncio.get_event_loop().stop()

    async def on_cmd_newLog(self, websocket, cmd_list, client_key):
        dir = cmd_list[1]
        if not os.path.exists(dir):
            os.mkdir(dir)
        self.logger = GUISocket.Utils.LOGGER
        self.logger.setPath(dir)
        self.logger.setPrefix('log')
        self.logger.new()
        await websocket.send('newLog')

    async def on_cmd_log(self, websocket, cmd_list, client_key):
        if self.logger is None:
            return
        self.logger.info(" ".join(cmd_list))
    
    async def on_cmd_getLog(self, websocket, cmd_list, client_key):
        if self.logger is None:
            await websocket.send('noLogger')
            return
        await websocket.send(self.logger.stream.getvalue())

    def log(self, content):
        if self.logger is None:
            return
        self.logger.info(content)
  
def main():
    '''
        入口函数, 可以指定websocket端口号, 用于GUI的访问
    '''
    handler = DAQHandler()
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', default=8000, help='The socket port from GUI connect to DAQ server')
    args = parser.parse_args()
    GUISocket.start(handler,args.port)

if __name__ == "__main__":
    main()
