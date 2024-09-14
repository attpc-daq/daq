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
import sensors

class SlowControlHandler(GUISocket.Utils.WebsocketHander):
    '''
    websocket handler 响应GUI命令
    '''
    def __init__(self):
        super(SlowControlHandler, self).__init__()
        self.sensors = None
    #—————————————————————————————————————————————————————————————
    #自定义函数应为协程函数，前缀为'on_cmd_'，参数列表不可变更
    #—————————————————————————————————————————————————————————————
    async def on_cmd_mailAddressAdd(self, websocket, msg_list, client_key):
        '''
        添加邮件地址
        '''
        if self.sensors is None:
            return
        if msg_list[1] in self.sensors.mailList:
            return
        self.sensors.mailList.append(msg_list[1])
        await websocket.send("mailAddress %s" % self.sensors.mailList)

    async def on_cmd_mailAddressRemove(self, websocket, msg_list, client_key):
        '''
        移除邮件地址
        '''
        if self.sensors is None:
            return
        if len(self.sensors.mailList) == 0:
            return
        if msg_list[1] not in self.sensors.mailList:
            return
        self.sensors.mailList.remove(msg_list[1])
        await websocket.send("mailAddress %s" % self.sensors.mailList)

    async def on_cmd_phoneNumberAdd(self, websocket, msg_list, client_key):
        '''
        添加手机号
        '''
        if self.sensors is None:
            return
        if msg_list[1] in self.sensors.phoneList:
            return
        self.sensors.phoneList.append(msg_list[1])
        await websocket.send("phoneNumber %s" % self.sensors.phoneList)

    async def on_cmd_phoneNumberRemove(self, websocket, msg_list, client_key):
        '''
        移除手机号
        '''
        if self.sensors is None:
            return
        if len(self.sensors.phoneList) == 0:
            return 
        if msg_list[1] not in self.sensors.phoneList:
            return
        self.sensors.phoneList.remove(msg_list[1])
        await websocket.send("phoneNumber %s" % self.sensors.phoneList)

    async def on_cmd_rootPeriodSet(self, websocket, msg_list, client_key):
        '''
        设置 root file 存储的最大时长
        '''
        self.sensors.rootPeriod = int(msg_list[1])
        await websocket.send("rootPeriodSet Done")

    async def on_cmd_targetpressureSet(self, websocket, msg_list, client_key):
        '''
        设置实验气体靶压力
        '''
        if self.sensors is None:
            return
        self.sensors.targetPressure = float(msg_list[1])
        
    async def on_cmd_targetgasflowSet(self, websocket, msg_list, client_key):
        '''
        设置实验气体靶流量
        '''
        if self.sensors is None:
            return
        self.sensors.targetGasflow = float(msg_list[1])

    async def on_cmd_pressureLowerLimitSet(self, websocket, msg_list, client_key):
        '''
        设置压力下限
        '''
        if self.sensors is None:
            return
        self.sensors.pressureLowerLimit = float(msg_list[1])
    
    async def on_cmd_pressureUpperLimitSet(self, websocket, msg_list, client_key):
        '''
        设置压力上限
        '''
        if self.sensors is None:
            return
        self.sensors.pressureUpperLimit = float(msg_list[1])

    async def on_cmd_initSensors(self, websocket, msg_list, client_key):
        self.sensors = sensors.Threading(self, msg_list)

    async def on_cmd_stopSensors(self, websocket, msg_list, client_key):
        self.sensors.stop()

    async def on_cmd_startSensors(self, websocket, msg_list, client_key):
        self.sensors.start()

    async def on_cmd_shutdownSensors(self, websocket, msg_list, client_key):
        # self.sensors._ser_Pressure.close()
        # self.sensors._ser_MFC.close()
        # self.sensors._ser_T_H.close()
        self.sensors = None
        await websocket.close()

    async def on_cmd_getPressure(self, websocket, msg_list, client_key):
        '''
        获取压力值
        '''
        if self.sensors is None:
            return
        await websocket.send("pressure %s" % self.sensors.pressure)
        
    async def on_cmd_getTemperature(self, websocket, msg_list, client_key):
        '''
        获取温度值
        '''
        if self.sensors is None:
            return
        await websocket.send("temperature %s" % self.sensors.temperature)
        
    async def on_cmd_getHumidity(self, websocket, msg_list, client_key):
        '''
        获取湿度值
        '''
        if self.sensors is None:
            return
        await websocket.send("humidity %s" % self.sensors.humidity)
        
    async def on_cmd_getGasFlow(self, websocket, msg_list, client_key):
        '''
        获取气体流量
        '''
        if self.sensors is None:
            return
        await websocket.send("gasFlow %s" % self.sensors.gasflow)

    async def on_cmd_getSensorsState(self, websocket, msg_list, client_key):
        '''
        获取传感器状态
        '''
        if self.sensors is None:
            return
        await websocket.send("sensorsState %s" % self.sensors.state)

def main():
    '''
        入口函数, 可以指定websocket端口号, 用于GUI的访问
    '''
    handler = SlowControlHandler()
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', default=8100, help='The socket port from GUI connect to Slow Control server')
    args = parser.parse_args()
    GUISocket.start(handler,args.port)

if __name__ == "__main__":
    main()
