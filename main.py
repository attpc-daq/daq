# -*- coding:utf-8 -*-

#import daq
import time
from daq import CMDHandler
from daq import WSServer

def main():
    #daq.startWSServer('0.0.0.0',8000)
    commandHandler=CMDHandler()
    CMDServer = WSServer(commandHandler,'0.0.0.0',8000)
    CMDServer.start()
    #WSServer(CMDHandler(),'0.0.0.0',8000).start()

    try:
        while CMDServer.state() == 'running':
            time.sleep(0.1)
    except KeyboardInterrupt:
        commandHandler.on_shutdown(CMDServer)
        
    CMDServer.join(5)

if __name__ == "__main__":
    main()