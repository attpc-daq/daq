# -*- coding:utf-8 -*-

#import daq
import time
from daq import CMDHandler
from daq import WSServer
from daq import StatusHandler

def main():
    MonitorHandler = StatusHandler()
    MonitorServer = WSServer(MonitorHandler,'0.0.0.0',8001)
    MonitorServer.start()

    commandHandler=CMDHandler(MonitorHandler)
    CMDServer = WSServer(commandHandler,'0.0.0.0',8000)
    CMDServer.start()

    try:
        while CMDServer.state() == 'running':
            time.sleep(0.1)
    except KeyboardInterrupt:
        commandHandler.on_shutdown(CMDServer)
        
    CMDServer.join(5)

if __name__ == "__main__":
    main()