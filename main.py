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
        self.outputDir = './output/'
        self.sitcpBufferFileSize = 1024*1024
    #—————————————————————————————————————————————————————————————
    #自定义函数应为协程函数，前缀为'on_cmd_'，参数列表不可变更
    #—————————————————————————————————————————————————————————————
    async def on_cmd_setrawdatafilesize(self, websocket, cmd_list, client_key):
        self.sitcpBufferFileSize = int(cmd_list[1])

    async def on_cmd_initsitcp(self, websocket, cmd_list, client_key):
        '''
        初始化SiTCP
        '''
        self.sitcp = ROOT.SiTCP()       
        # self.sitcp.connectDevice("0.0.0.0",8002)
        self.sitcp.setDir(self.outputDir)
        self.sitcp.setDataProcessHost(8802)
        self.sitcp.setFileMaxSize(16*1024*1024)
    
    async def on_cmd_initdataprocessor(self, websocket, cmd_list, client_key):
        '''
        初始化DataProcessor
        '''
        self.dataProcessor = ROOT.DataProcessor()
        self.dataProcessor.setDir(self.outputDir)
        self.dataProcessor.setDataPort(8802)
        self.dataProcessor.setQAPort(8803)
        self.dataProcessor.setEventSave(False)
        self.dataProcessor.setRawEventSave(True)
        self.dataProcessor.setQA(True)
        self.dataProcessor.generateParameters(10)
        self.dataProcessor.setFileEvents(1000)


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
    
    async def on_cmd_startdataprocessor(self, websocket, cmd_list, client_key):
        '''
        '''
        self.dataProcessor.run()

    async def on_cmd_stopdataprocessor(self, websocket, cmd_list, client_key):
        '''
        '''
        self.dataProcessor.stop()

    async def on_cmd_connectdevice(self, websocket, cmd_list, client_key):
        '''
        连接设备
        '''
        if len(cmd_list) == 1:
            self.sitcp.connectDevice("192.168.10.16",4660)
        else:
            self.sitcp.connectDevice(cmd_list[1],int(cmd_list[2]))

    async def on_cmd_selftrigger(self, websocket, cmd_list, client_key):
        self.sitcp.sendToDevice(bytes.fromhex('0A1020384150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('0610203A4150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('041020314150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('001020304150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('041020384150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('001028314050607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('081020384150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('021020394F5F6F7F83'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('0410203A4150607083'))
        time.sleep(1)

    async def on_cmd_initeventqa(self, websocket, cmd_list, client_key):

        self.eventQA = ROOT.EventQA(8808)
        self.eventQA.setMessageHost(8803,"0.0.0.0")
        file = open(os.path.join(self.outputDir,'./eventParameters.json'), 'r')
        settingJson = file.read()
        self.eventQA.updateSettings(settingJson)
        self.eventQA.setDir(self.outputDir)
        
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
        self.outputDir = cmd_list[1]
        if not self.outputDir.endswith("/"):
            self.outputDir = self.outputDir + "/"
        if not os.path.exists(self.outputDir):
            os.mkdir(self.outputDir)
        pass

    async def on_cmd_stopdata(self, websocket, cmd_list, client_key):
        self.sitcp.sendToDevice(bytes.fromhex('0410203A4050607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('081020384150607083'))
        time.sleep(1)

    async def on_cmd_startdata(self, websocket, cmd_list, client_key):
        self.sitcp.sendToDevice(bytes.fromhex('0410203A4150607083'))
        time.sleep(1)

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

    async def on_cmd_send2device(self, websocket, cmd_list, client_key):
        self.sitcp.sendToDevice(bytes.fromhex(str(cmd_list[1])))

    async def on_cmd_cleardir(self, websocket, cmd_list, client_key):
        for filename in os.listdir(self.outputDir):
            if filename.endswith(".dat") or filename.endswith(".root"):
                 os.remove(os.path.join(self.outputDir, filename))

        if os.path.exists("/dev/shm/sem.REP"):
            os.remove("/dev/shm/sem.REP")

    async def on_cmd_getqalist(self, websocket, cmd_list, client_key):
        await websocket.send(self.eventQA.getList())

    async def on_cmd_setupthreshold(self, websocket, cmd_list, client_key): 
        self.sitcp.sendToDevice(bytes.fromhex('0A1020384150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('0610203A4150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('041020314150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('001020304150607083'))
        time.sleep(1)
        self.sitcp.sendToDevice(bytes.fromhex('041020384150607083'))
        time.sleep(1)

        with open(os.path.join(self.outputDir,"thresholdes.json"),'r') as file:
            content = file.read()
            msgJson = json.loads(content)

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
    parser.add_argument('--gui_port', default=8809, help='The socket port from GUI connect to DAQ server')
    args = parser.parse_args()
    GUISocket.start(handler,args.gui_port)

if __name__ == "__main__":
    main()
