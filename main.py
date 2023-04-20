# -*- coding:utf-8 -*-
import argparse
import GUISocket
import ROOT
import sys
import time
import asyncio
import os
import json

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gInterpreter.Declare('#include "PacketDecoder.h"')
ROOT.gInterpreter.Declare('#include "RawDataProcessor.h"')
ROOT.gInterpreter.Declare('#include "ParameterGenerator.h"')
ROOT.gInterpreter.Declare('#include "Event.h"')
ROOT.gInterpreter.Declare('#include "EventMaker.h"')
ROOT.gInterpreter.Declare('#include "EventQA.h"')
ROOT.gInterpreter.Declare('#include "RawEvent.h"')
ROOT.gInterpreter.Declare('#include "RawEventProcessor.h"')
ROOT.gInterpreter.Declare('#include "TMessageBuffer.h"')
ROOT.gInterpreter.Declare('#include "TMessageSocket.h"')

ROOT.gSystem.Load("libSiTCP.so")
ROOT.gSystem.Load("libPacketDecoder.so")
ROOT.gSystem.Load("libRawDataProcessor.so")
ROOT.gSystem.Load("libParameterGenerator.so")
ROOT.gSystem.Load("libEvent.so")
ROOT.gSystem.Load("libEventMaker.so")
ROOT.gSystem.Load("libEventQA.so")
ROOT.gSystem.Load("libRawEvent.so")
ROOT.gSystem.Load("libRawEventProcessor.so")
ROOT.gSystem.Load("libTMessageBuffer.so")
ROOT.gSystem.Load("libTMessageSocket.so")
ROOT.gSystem.Load("libHist.so")



# import websockets
# import os
# import secrets
# import time
# import math
# import json


class DAQHandler(GUISocket.Utils.WebsocketHander):
    '''
    websocket handler 响应GUI命令
    '''
    def __init__(self):
        super(DAQHandler, self).__init__()

    #—————————————————————————————————————————————————————————————
    #自定义函数应为协程函数，前缀为'on_cmd_'，参数列表不可变更
    #—————————————————————————————————————————————————————————————
    async def on_cmd_initsitcp(self, websocket, cmd_list, client_key):
        '''
        初始化SiTCP
        '''
        self.sitcp = ROOT.SiTCP()
        # self.sitcp.connectToDevice("0.0.0.0",8002)
        self.sitcp.setDir("./output")
        self.sitcp.setSocketBufferSize(1024*1024)
        self.sitcp.setFileMaxSize(10*1024*1024)
        self.sitcp.setFilePrefix("packet")

    async def on_cmd_startsitcp(self, websocket, cmd_list, client_key):
        '''
        启动SiTCP
        '''
        self.sitcp.run()

    async def on_cmd_stopsitcp(self, websocket, cmd_list, client_key):
        '''
        停止SiTCP
        '''
        self.sitcp.stop()

    async def on_cmd_connect2device(self, websocket, cmd_list, client_key):
        '''
        连接设备
        '''
        print("connect2device")
        self.sitcp.connectToDevice("0.0.0.0",8002)

    async def on_cmd_initrawdataprocessor(self, websocket, cmd_list, client_key):
        '''
        初始化RawDataProcessor
        '''
        self.rawDataProcessor = ROOT.RawDataProcessor()
        self.rawDataProcessor.setRawDataDir("./output")
        self.rawDataProcessor.setRawDataFilePrefix("packet")
        self.rawDataProcessor.setReadingTempFileName("reading.dat")
        self.rawDataProcessor.setOutputFileEvents(100)
        self.rawDataProcessor.setOutputDir("./output")
        self.rawDataProcessor.setOutputFilePrefix("RawEvent")
        self.rawDataProcessor.setWritingTempFileName("writing.root")
        self.rawDataProcessor.setRawEventTreeName("raw_event_tree")
        self.rawDataProcessor.setRawEventBranchName("raw_event")
        self.rawDataProcessor.setSocketPort(8020)


    async def on_cmd_startrawdataprocessor(self, websocket, cmd_list, client_key):
        '''
        启动RawDataProcessor
        '''
        if len(cmd_list)>1 and cmd_list[1]=='false':
            self.rawDataProcessor.run(False)
        else:
            self.rawDataProcessor.run()
    
    async def on_cmd_stoprawdataprocessor(self, websocket, cmd_list, client_key):
        '''
        停止RawDataProcessor
        '''
        self.rawDataProcessor.stop()

    async def on_cmd_initparametergenerator(self, websocket, cmd_list, client_key):
        self.parameterGenerator = ROOT.ParameterGenerator()
        self.parameterGenerator.setRawDataDir("./output")
        self.parameterGenerator.setRawDataFilePrefix("packet")
        self.parameterGenerator.setReadingTempFileName("reading.dat")
        self.parameterGenerator.setOutputDir("./")
        self.parameterGenerator.setOutputFileName("thresholdes.json","eventParameters.json")

    async def on_cmd_startparametergenerator(self, websocket, cmd_list, client_key):
        await self.notifymonitor("starting parametergeneration...")
        self.parameterGenerator.run(3)
        while self.parameterGenerator.isFinish() is False:
            time.sleep(1)
        await self.notifymonitor("parameter generation finish")

    async def on_cmd_stopparametergenerator(self, websocket, cmd_list, client_key):
        self.parameterGenerator.stop()

    async def on_cmd_initraweventprocessor(self, websocket, cmd_list, client_key):
        self.rawEventProcessor = ROOT.RawEventProcessor()
        file = open('./eventParameters.json', 'r')
        settingJson = file.read()
        self.rawEventProcessor.updateSettings(settingJson)
        self.rawEventProcessor.setOutputFilePrefix("Event")
        self.rawEventProcessor.setDir("output")
        self.rawEventProcessor.setRawEventFilePrefix("RawEvent")
        self.rawEventProcessor.setRawTreeName("raw_event_tree")
        self.rawEventProcessor.setRawBranchName("raw_event")
        self.rawEventProcessor.setEventTreeName("event_tree")
        self.rawEventProcessor.setEventBranchName("event")
        
    async def on_cmd_startraweventprocessor(self, websocket, cmd_list, client_key):
        self.rawEventProcessor.run()

    async def on_cmd_stopraweventprocessor(self, websocket, cmd_list, client_key):
        self.rawEventProcessor.stop()
    
    async def on_cmd_initeventqa(self, websocket, cmd_list, client_key):

        self.eventQA = ROOT.EventQA()
        self.eventQA.setMessageHost(8020,"0.0.0.0")
        file = open('./eventParameters.json', 'r')
        settingJson = file.read()
        self.eventQA.updateSettings(settingJson)
        
    async def on_cmd_starteventqa(self, websocket, cmd_list, client_key):
        self.eventQA.run()

    async def on_cmd_stopeventqa(self, websocket, cmd_list, client_key):
       self.eventQA.stop()
    
    async def on_cmd_getQA(self, websocket, cmd_list, client_key):
        if len(cmd_list)==1:
            return
        str = self.eventQA.get(cmd_list[1])
        await websocket.send(str)

    async def on_cmd_setdir(self, websocket, cmd_list, client_key):
        '''
        设置文件保存路径
        '''
        # self.sitcp.setDir("./output")
        pass

    async def on_cmd_generateparameter(self, websocket, cmd_list, client_key):

        pass

    async def on_cmd_startdata(self, websocket, cmd_list, client_key):
        # self.sitcp.sendToDevice(bytes.fromhex('0A1020384150607083'))
        # time.sleep(0.01)
        # self.sitcp.sendToDevice(bytes.fromhex('0610203A4150607083'))
        # time.sleep(0.01)
        pass

    async def on_cmd_shutdown(self, websocket, cmd_list, client_key):
        asyncio.get_event_loop().stop()

    async def on_cmd_setlog(self, websocket, cmd_list, client_key):
        if not os.path.exists(cmd_list[1]):
            os.mkdir(cmd_list[1])
        GUISocket.Utils.LOGGER.setPath(cmd_list[1])
        GUISocket.Utils.LOGGER.setPrefix(cmd_list[2])
        GUISocket.Utils.LOGGER.new()
    async def on_cmd_log(self, websocket, cmd_list, client_key):
        GUISocket.Utils.LOGGER.info(" ".join(cmd_list))

    async def on_cmd_send2device(self, websocket, cmd_list, client_key):
        msg = bytes.fromhex(str(cmd_list[1]))
        self.sitcp.sendToDevice(msg)

    async def on_cmd_setupthreshold(self, websocket, cmd_list, client_key):
        self.sitcp.sendToDevice(bytes.fromhex('0A1020384150607083'))
        time.sleep(0.01)
        self.sitcp.sendToDevice(bytes.fromhex('0610203A4150607083'))
        time.sleep(0.01)
        self.sitcp.sendToDevice(bytes.fromhex('041020314150607083'))
        time.sleep(0.01)
        self.sitcp.sendToDevice(bytes.fromhex('001020304150607083'))
        time.sleep(0.01)
        self.sitcp.sendToDevice(bytes.fromhex('041020384150607083'))
        time.sleep(0.01)

        #with open(cmd_list[1],'r') as file:
        with open("thresholdes.json",'r') as file:
            msgJson = json.loads(file)

        GUISocket.Utils.LOGGER.info("SiTCP threshold setting:")
        for i in range(msgJson['count']):
            for key in msgJson[str(i)]:
                for k, v in key.items():
                    cmd = bytes.fromhex(v)
                    try:
                        self.sitcp.sendToDevice(cmd)
                        time.sleep(0.01)
                        GUISocket.Utils.LOGGER.info(f"{k}: {v}")
                    except:
                        await self.notifymonitor("SiTCP threshold setting error!")
            if i%100 == 0:
                await self.notifymonitor("SiTCP threshold setting..."+str(int(i/20.48))+"%")
        await self.notifymonitor("SiTCP threshold setting done!")
    

def main():
    '''
        入口函数, 可以指定websocket端口号, 用于GUI的访问
    '''
    handler = DAQHandler()
    parser = argparse.ArgumentParser()
    parser.add_argument('--gui_port', default=8003, help='The socket port from GUI connect to DAQ server')
    args = parser.parse_args()
    GUISocket.start(handler,args.gui_port)

if __name__ == "__main__":
    main()
