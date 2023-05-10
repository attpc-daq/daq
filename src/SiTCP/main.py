# -*- coding:utf-8 -*-
import argparse
import ROOT
import sys
import time

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gSystem.Load("libSiTCP.so")

def main():
    sitcp = ROOT.SiTCP()
    sitcp.connectToDevice("0.0.0.0",8801)
    sitcp.setDir("./output_test1")
    sitcp.clearDir()
    sitcp.sendToDevice('0xFFFFFFFF')
    sitcp.setDataHost(8802,"localhost")
    sitcp.setSocketBufferSize(1024*1024)
    sitcp.setFileMaxSize(1024*1024*1024)
    sitcp.run()
    sitcp.getState()
    sitcp.getConnectionState()
    sitcp.getRate()
    sitcp.getNTasks()

    pass
    time.sleep(100)
    sitcp.stop()

    
if __name__ == "__main__":
    main()