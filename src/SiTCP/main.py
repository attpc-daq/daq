# -*- coding:utf-8 -*-
import argparse
import ROOT
import sys

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gSystem.Load("libSiTCP.so")

def main():
    sitcp = ROOT.SiTCP()
    sitcp.connectToDevice("0.0.0.0",8002)
    sitcp.setDir("./output_test1")
    # sitcp.sendToDevice('0xFFFFFFFF');
    sitcp.setSocketBufferSize(1024*1024)
    sitcp.setFileMaxSize(1024*1024*1024)
    sitcp.run()
    # sitcp.sendToDevice('0xFFFFFFFF');

    pass
    sitcp.stop()

    
if __name__ == "__main__":
    main()