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

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gInterpreter.Declare('#include "PacketDecoder.h"')
ROOT.gInterpreter.Declare('#include "ParameterGenerator.h"')
ROOT.gInterpreter.Declare('#include "Event.h"')
ROOT.gInterpreter.Declare('#include "EventConverter.h"')
ROOT.gInterpreter.Declare('#include "EventQA.h"')
ROOT.gInterpreter.Declare('#include "RawEvent.h"')
ROOT.gInterpreter.Declare('#include "TMessageBuffer.h"')
ROOT.gInterpreter.Declare('#include "TMessageSocket.h"')
ROOT.gInterpreter.Declare('#include "DataProcessor.h"')

ROOT.gSystem.Load("libSiTCP.so")
ROOT.gSystem.Load("libPacketDecoder.so")
ROOT.gSystem.Load("libParameterGenerator.so")
ROOT.gSystem.Load("libEvent.so")
ROOT.gSystem.Load("libEventConverter.so")
ROOT.gSystem.Load("libEventQA.so")
ROOT.gSystem.Load("libRawEvent.so")
ROOT.gSystem.Load("libTMessageBuffer.so")
ROOT.gSystem.Load("libTMessageSocket.so")
ROOT.gSystem.Load("libHist.so")
ROOT.gSystem.Load("libDataProcessor.so")

class DAQHandler(GUISocket.Utils.WebsocketHander):
    '''
    websocket handler 响应GUI命令
    '''
    def __init__(self):
        super(DAQHandler, self).__init__()
        self.sitcp = [None,None]
        self.dataProcessor = None
        self.eventQA = None
        self.parameterGenerated = True

        self.outputDir = './output/'
        self.FileNEvents = 100
        #update: by whk
        self.Wvalue = 30.0 #unit: eV
        self.Vdrift = 10.0 #unit: mm/ns
        self.fpc2_vector = ROOT.vector(ROOT.map('string', 'int'))()
        # self._SiTCP1_address = "192.168.10.16"
        # self._SiTCP1_port=4660
        # self._SiTCP2_address = "192.168.10.17"
        # self._SiTCP2_port=4660
        
    #—————————————————————————————————————————————————————————————
    #自定义函数应为协程函数，前缀为'on_cmd_'，参数列表不可变更
    #—————————————————————————————————————————————————————————————
    async def on_cmd_send_to_SiTCP1(self, websocket, msg_list, client_key):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self.sitcp[0].getIP(), self.sitcp[0].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            message = "Can not connect to SiTCP port"
            await websocket.send(message)
            return
        msg = bytes.fromhex(str(msg_list[1]))
        sock.send(msg)
        message = str(msg_list[1]) + " done"
        await websocket.send(message)
        sock.close()
    
    async def on_cmd_send_to_SiTCP2(self, websocket, msg_list, client_key):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self.sitcp[1].getIP(), self.sitcp[1].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            message = "Can not connect to SiTCP port"
            await websocket.send(message)
            return
        msg = bytes.fromhex(str(msg_list[1]))
        sock.send(msg)
        message = str(msg_list[1]) + " done"
        await websocket.send(message)
        sock.close()

    async def on_cmd_generateParameter(self, websocket, cmd_list, client_key):
        self.dataProcessor.generateParameters(int(cmd_list[1]))
        self.parameterGenerated = False

    async def on_cmd_turnOnRawEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setRawEventSave(True)

    async def on_cmd_turnOffRawEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setRawEventSave(False)

    async def on_cmd_turnOnEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setEventSave(True)

    async def on_cmd_turnOffEventSave(self, websocket, cmd_list, client_key):
        self.dataProcessor.setEventSave(False)

    async def on_cmd_turnOnQAData(self, websocket, cmd_list, client_key):
        self.dataProcessor.setQA(True)

    async def on_cmd_turnOffQAData(self, websocket, cmd_list, client_key):
        self.dataProcessor.setQA(False)

    async def on_cmd_startDecoder(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].enableDecoder()
        if self.sitcp[1] is not None:
            self.sitcp[1].enableDecoder()

    async def on_cmd_stopDecoder(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            self.sitcp[0].disableDecoder()
        if self.sitcp[1] is not None:
            self.sitcp[1].disableDecoder()

    async def on_cmd_dumpSitcp(self, websocket, cmd_list, client_key):
        if self.sitcp[0] is not None:
            await websocket.send("dataRate1 "+str(self.sitcp[0].getRate()))
            await websocket.send("nTask1 "+str(self.sitcp[0].getNTasks()))
            await websocket.send("decoderState1 "+str(self.sitcp[0].getDecoderState()))
            await websocket.send("daqState1 "+str(self.sitcp[0].getState()))
        else:
            await websocket.send("nositcp1")

        if self.sitcp[1] is not None:
            await websocket.send("dataRate2 "+str(self.sitcp[1].getRate()))
            await websocket.send("nTask2 "+str(self.sitcp[1].getNTasks()))
            await websocket.send("decoderState2 "+str(self.sitcp[1].getDecoderState()))
            await websocket.send("daqState2 "+str(self.sitcp[1].getState()))
        else:
            await websocket.send("nositcp2")

    async def on_cmd_dumpDP(self, websocket, cmd_list, client_key):
        if self.dataProcessor is not None:
            await websocket.send("eventRate "+str(self.dataProcessor.getRate()))
            await websocket.send("raweventFiles "+str(self.dataProcessor.getNRawEventFiles()))
            await websocket.send("DPState "+str(self.dataProcessor.getState()))
            if self.parameterGenerated is False:
                npar = self.dataProcessor.getNMakePar()
                if npar <0 :
                    return
                await websocket.send("parameterEvent "+str(npar))
                if  npar == 0:
                    self.parameterGenerated = True

        else:
            await websocket.send("NODP")

    async def on_cmd_dumpQA(self, websocket, cmd_list, client_key):
        if self.eventQA is not None:
            await websocket.send("QAEventRate "+str(self.eventQA.getRate()))
            await websocket.send("QAState "+str(self.eventQA.getState()))
        else:
            await websocket.send("NOQA")
        
    async def on_cmd_setEventsPerFile(self, websocket, cmd_list, client_key):
        self.FileNEvents = int(cmd_list[1])

    async def on_cmd_initSitcp(self, websocket, cmd_list, client_key):
        '''
        初始化SiTCP
        '''
        if self.sitcp[0] is None:
            self.sitcp[0] = ROOT.SiTCP()
            self.sitcp[0].setupServerAddress(cmd_list[1], int(cmd_list[2]))
            self.sitcp[0].setDir(cmd_list[3])
            self.sitcp[0].setDataProcessHost(int(cmd_list[8]))
            self.sitcp[0].setFileMaxSize(int(cmd_list[7]))
            self.sitcp[0].disableDecoder()

        if self.sitcp[1] is None:
            self.sitcp[1] = ROOT.SiTCP()
            self.sitcp[1].setupServerAddress(cmd_list[4], int(cmd_list[5]))
            self.sitcp[1].setDir(cmd_list[6])
            self.sitcp[1].setDataProcessHost(int(cmd_list[8]))
            self.sitcp[1].setFileMaxSize(int(cmd_list[7]))
            self.sitcp[1].disableDecoder()
    
    async def on_cmd_setBufferFileSize(self, websocket, cmd_list, client_key):
        self.sitcp[0].setFileMaxSize(int(cmd_list[1]))
        self.sitcp[1].setFileMaxSize(int(cmd_list[1]))
        await websocket.send("setBufferFileSize done!")
    
      
    async def on_cmd_initdataprocessor(self, websocket, cmd_list, client_key):
        '''
        初始化DataProcessor
        '''
        self.dataProcessor = ROOT.DataProcessor()
        self.dataProcessor.setDir(cmd_list[1])
        self.dataProcessor.setDataPort(int(cmd_list[2]))#TODO:数据存储进程端口
        self.dataProcessor.setQAPort(int(cmd_list[3]))#TODO:QA进程端口
        self.dataProcessor.setEventSave(False)
        self.dataProcessor.setRawEventSave(False)
        self.dataProcessor.setQA(False)
        self.dataProcessor.setFileEvents(int(cmd_list[4]))

    #update: by whk
    async def on_cmd_setWvalueAndVdrift(self, websocket, cmd_list, client_key):
        self.Wvalue = float(cmd_list[1])
        self.Vdrift = float(cmd_list[2])
        self.dataProcessor.setWValue(self.Wvalue)
        self.dataProcessor.setVdrift(self.Vdrift)
    async def on_cmd_setGainFile(self, websocket, cmd_list, client_key):
        self.electronicFile = str(cmd_list[1])
        self.micromegasFile = str(cmd_list[2])
        self.dataProcessor.setElectronicFile(self.electronicFile)
        self.dataProcessor.setMicromegasFile(self.micromegasFile)
    async def on_cmd_setFPC2(self, websocket, cmd_list, client_key):
        self.fpc2_vector.clear()
        fpc2_list = ast.literal_eval(str(cmd_list[1]))
        # 将Python列表的数据填充到ROOT的vector对象中
        for item in fpc2_list:
            map_obj = ROOT.map('string', 'int')()
            for key, value in item.items():
                map_obj[key] = int(value)
            self.fpc2_vector.push_back(map_obj)
            
        self.dataProcessor.setFPC2(self.fpc2_vector)
                
    async def on_cmd_startdataprocessor(self, websocket, cmd_list, client_key):
        '''
        '''
        self.dataProcessor.run()

    async def on_cmd_stopdataprocessor(self, websocket, cmd_list, client_key):
        '''
        '''
        self.dataProcessor.stop()

    async def on_cmd_selftrigger(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0A1020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0A1020384150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0610203A4150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0610203A4150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '041020314150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '041020314150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '001020304150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '001020304150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '041020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '041020384150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '001028314050607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '001028314050607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '081020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '081020384150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '021020394F5F6F7F83'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '021020394F5F6F7F83'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0410203A4150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0410203A4150607083'], client_key)
        time.sleep(1)
        await websocket.send("selftrigger done!")

    async def on_cmd_initeventqa(self, websocket, cmd_list, client_key):

        self.eventQA = ROOT.EventQA(int(cmd_list[1]))
        self.eventQA.setMessageHost(int(cmd_list[2]),cmd_list[3])
        file = open(os.path.join(self.outputDir,'./eventParameters.json'), 'r')
        settingJson = file.read()
        self.eventQA.updateSettings(settingJson)
        self.eventQA.setDir(self.outputDir)
        
    async def on_cmd_starteventqa(self, websocket, cmd_list, client_key):
        self.eventQA.run()

    async def on_cmd_stopeventqa(self, websocket, cmd_list, client_key):
       self.eventQA.stop()
    #    self.eventQA = None
    
    async def on_cmd_getQA(self, websocket, cmd_list, client_key):
        if len(cmd_list)==1:
            return
        str = self.eventQA.get(cmd_list[1])
        await websocket.send(str)

    async def on_cmd_setdir(self, websocket, cmd_list, client_key):
        '''
        设置文件保存路径
        '''
        self.outputDir = cmd_list[1]
        if not self.outputDir.endswith("/"):
            self.outputDir = self.outputDir + "/"
        if not os.path.exists(self.outputDir):
            os.mkdir(self.outputDir)
        pass

    async def on_cmd_stopdata(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0410203A4050607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0410203A4050607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '081020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '081020384150607083'], client_key)
        time.sleep(1)

    async def on_cmd_startdata(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0410203A4150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0410203A4150607083'], client_key)
        if self.sitcp[0] is not None:
            self.sitcp[0].run()
        if self.sitcp[1] is not None:
            self.sitcp[1].run()
        # time.sleep(1)

    async def on_cmd_shutdown(self, websocket, cmd_list, client_key):
        asyncio.get_event_loop().stop()

    async def on_cmd_newlog(self, websocket, cmd_list, client_key):
        if not os.path.exists(self.outputDir):
            os.mkdir(self.outputDir)
        GUISocket.Utils.LOGGER.setPath(self.outputDir)
        GUISocket.Utils.LOGGER.setPrefix('log')
        GUISocket.Utils.LOGGER.new()

    async def on_cmd_log(self, websocket, cmd_list, client_key):
        GUISocket.Utils.LOGGER.info(" ".join(cmd_list))

    async def on_cmd_setfeehitmode(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', cmd_list[1]], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', cmd_list[1]], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', cmd_list[2]], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', cmd_list[2]], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', cmd_list[3]], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', cmd_list[3]], client_key)
        time.sleep(1)
        await websocket.send('setfeehitmode done!')

    async def on_cmd_setfeenchannel(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', cmd_list[1]], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', cmd_list[1]], client_key)
        time.sleep(1)
        await websocket.send('setfeenchannel done!')

    async def on_cmd_setfeeslope(self, websocket, cmd_list, client_key):
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', cmd_list[1]], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', cmd_list[1]], client_key)
        time.sleep(1)
        await websocket.send('setfeeslope done!')

    # async def on_cmd_send2device(self, websocket, cmd_list, client_key):
    #     await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP', cmd_list[1]], client_key)
    #     # self.sitcp.sendToDevice(bytes.fromhex(str(cmd_list[1])))

    async def on_cmd_cleardir(self, websocket, cmd_list, client_key):
        for filename in os.listdir(self.outputDir):
            if filename.endswith(".dat") or filename.endswith(".root"):
                 os.remove(os.path.join(self.outputDir, filename))

        if os.path.exists("/dev/shm/sem.REP"):
            os.remove("/dev/shm/sem.REP")

    async def on_cmd_getqalist(self, websocket, cmd_list, client_key):
        await websocket.send("QAList "+self.eventQA.getList())
  
    async def on_cmd_setupthreshold(self, websocket, cmd_list, client_key):

        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0410203A4050607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0410203A4050607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '081020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '081020384150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0A1020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0A1020384150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '0610203A4150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '0610203A4150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '041020314150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '041020314150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '001020304150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '001020304150607083'], client_key)
        time.sleep(1)
        await self.on_cmd_send_to_SiTCP1(websocket,['send_to_SiTCP1', '041020384150607083'], client_key)
        await self.on_cmd_send_to_SiTCP2(websocket,['send_to_SiTCP2', '041020384150607083'], client_key)
        time.sleep(1)

        sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock1.settimeout(2.0)
        sock2.settimeout(2.0)
        try:
            sock1.connect((self.sitcp[0].getIP(), self.sitcp[0].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            message = "Can not connect to SiTCP1 port"
            await websocket.send(message)
            return
        try:
            sock2.connect((self.sitcp[1].getIP(), self.sitcp[1].getPort()))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            message = "Can not connect to SiTCP2 port"
            await websocket.send(message)
            return

        with open(cmd_list[1],'r') as file:
            content = file.read()
            msgJson = json.loads(content)

        GUISocket.Utils.LOGGER.info("SiTCP threshold setting:")
        
        for i in range(msgJson['count']):
            for key in msgJson[str(i)]:
                for k, v in key.items():
                    cmd = bytes.fromhex(v)
                    try:
                        sock1.send(cmd)
                        sock2.send(cmd)
                        time.sleep(0.001)
                        GUISocket.Utils.LOGGER.info(f"{k}: {v}")
                    except:
                        await self.notifymonitor("SiTCP threshold setting error!")
            if i%100 == 0:
                await websocket.send("thresholdsetting "+str(int(i/20.48))+"%")
                # await self.notifymonitor("thresholdsetting "+str(int(i/20.48))+"%")
        await websocket.send("thresholdsetting done!")
        # await self.notifymonitor("thresholdsetting done!")

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
