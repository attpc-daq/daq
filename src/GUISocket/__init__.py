# -*- coding:utf-8 -*-
__version__ = '0.0.1'
from . import WebSocket

def start(handler, host_port):
    '''
    daq 模块入口函数，启动websocket服务线程
    '''
    server = WebSocket.Server(handler, host_port, '0.0.0.0')
    server.start()
    # server.join(5)
    print("WebSocket Server started at port %s" % host_port)