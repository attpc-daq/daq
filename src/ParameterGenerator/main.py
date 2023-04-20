# -*- coding:utf-8 -*-
import argparse
import ROOT
import sys
import time

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gInterpreter.Declare('#include "PacketDecoder.h"')
ROOT.gInterpreter.Declare('#include "RawDataProcessor.h"')
ROOT.gInterpreter.Declare('#include "ParameterGenerator.h"')

ROOT.gSystem.Load("libSiTCP.so")
ROOT.gSystem.Load("libPacketDecoder.so")
ROOT.gSystem.Load("libRawDataProcessor.so")
ROOT.gSystem.Load("libParameterGenerator.so")



def main():
    sitcp = ROOT.SiTCP()
    sitcp.connectToDevice("0.0.0.0",8002)
    sitcp.setDir("./output_test")
    sitcp.setSocketBufferSize(1024*1024)
    sitcp.setFileMaxSize(10*1024*1024)
    sitcp.setFilePrefix("packet")

    rawDataProcessor = ROOT.RawDataProcessor()
    rawDataProcessor.setRawDataDir("./output_test")
    rawDataProcessor.setRawDataFilePrefix("packet")
    rawDataProcessor.setReadingTempFileName("reading.dat")
    rawDataProcessor.setOutputFileEvents(10)
    rawDataProcessor.setOutputDir("./output_test")
    rawDataProcessor.setOutputFilePrefix("RawEvent")
    rawDataProcessor.setWritingTempFileName("writing.root")

    

    processor = ROOT.ParameterGenerator()
    processor.setRawDataDir("./output_test")
    processor.setRawDataFilePrefix("packet")
    processor.setReadingTempFileName("reading.dat")
    processor.setOutputDir("./")
    processor.setOutputFileName("thresholdes.json","eventParameters.json")

    sitcp.run()
    rawDataProcessor.run()
    processor.run(2)

    while processor.isFinish() is False:
        time.sleep(1)

    pass
    processor.stop()
    sitcp.stop()
    rawDataProcessor.stop()


if __name__ == "__main__":
    main()