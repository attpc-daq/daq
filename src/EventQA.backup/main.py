# -*- coding:utf-8 -*-
import argparse
import ROOT
import sys
import time

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "RawEvent.h"')
ROOT.gInterpreter.Declare('#include "Event.h"')
ROOT.gInterpreter.Declare('#include "EventMaker.h"')
ROOT.gInterpreter.Declare('#include "RawEventProcessor.h"')
ROOT.gInterpreter.Declare('#include "EventQA.h"')

ROOT.gSystem.Load("libRawEvent.so")
ROOT.gSystem.Load("libEvent.so")
ROOT.gSystem.Load("libEventMaker.so")
ROOT.gSystem.Load("libRawEventProcessor.so")
ROOT.gSystem.Load("libEventQA.so")
ROOT.gSystem.Load("libHist.so")

def main():
    sitcp = ROOT.SiTCP()
    sitcp.connectToDevice("0.0.0.0",8002)
    sitcp.setDir("./output_test1")
    sitcp.setSocketBufferSize(1024*1024)
    sitcp.setFileMaxSize(1*1024*1024)
    sitcp.run()

    rawData = ROOT.RawDataProcessor()
    rawData.setRawDataDir("output_test1")
    rawData.setRawDataFilePrefix("packet")
    rawData.setReadingTempFileName("reading.dat")
    rawData.setOutputFileEvents(100)
    rawData.setOutputDir("output_test1")
    rawData.setOutputFilePrefix("RawEvent")
    rawData.setWritingTempFileName("writing.root")
    rawData.setRawEventTreeName("raw_event_tree")
    rawData.setRawEventBranchName("raw_event")
    rawData.setSocketPort(8020)
    rawData.run()

    # eventPr = ROOT.RawEventProcessor()

    # file = open('/storage/eventSetting.json', 'r')
    # settingJson = file.read()
    # eventPr.updateSettings(settingJson)
    # eventPr.setOutputFilePrefix("Event")
    # eventPr.setDir("output_test1")
    # eventPr.setRawEventFilePrefix("RawEvent")
    # eventPr.setRawTreeName("raw_event_tree")
    # eventPr.setRawBranchName("raw_event")
    # eventPr.setEventTreeName("event_tree")
    # eventPr.setEventBranchName("event")
    # eventPr.setSocketPort(8020)
    # eventPr.run()

    processor = ROOT.EventQA()
    processor.setMessageHost(8020,"0.0.0.0")
    file = open('/storage/eventParameters.json', 'r')
    settingJson = file.read()
    processor.updateSettings(settingJson)

    processor.run()

    processor.get("track_2D")
    pass

    processor.stop()
    # eventPr.stop()
    rawData.stop()
    sitcp.stop()



if __name__ == "__main__":
    main()