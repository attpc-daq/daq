# -*- coding:utf-8 -*-
import argparse
import GUISocket
import sys
import time
import asyncio
import os
import json
import numpy as np
import ast
import socket
import psutil
import docker

sys.path.append('/usr/local/lib')

# 初始化 Docker 客户端
client = docker.from_env()

class DAQHandler(GUISocket.Utils.WebsocketHander):
    '''
    websocket handler 响应GUI命令
    '''
    def __init__(self, debug):
        super(DAQHandler, self).__init__()
        self.debug = debug

    async def on_cmd_restart(self, websocket, msg_list, client_key):
        try:
            container = client.containers.get("tpc_main_container")
            container.restart()
            #container.stop()
            #container.remove()
            #container = client.containers.get("tpc_main_container")
            #container.remove()
        except Exception as e:
            print(f"Error stopping container: {e}")
        
        #try:
        #    container = client.containers.run("rootproject/root", name="tpc_main_container", detach=True)
        #except Exception as e:
        #    print(f"Error starting container: {e}")

        await websocket.send("done")
  
def main():
    '''
        入口函数, 可以指定websocket端口号, 用于GUI的访问
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', default=8080)
    parser.add_argument('--debug', action='store_true')
    args = parser.parse_args()
    handler = DAQHandler(args.debug)
    GUISocket.start(handler,args.port)

if __name__ == "__main__":
    main()
