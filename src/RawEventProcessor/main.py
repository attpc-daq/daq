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

ROOT.gSystem.Load("libRawEvent.so")
ROOT.gSystem.Load("libEvent.so")
ROOT.gSystem.Load("libEventMaker.so")
ROOT.gSystem.Load("libRawEventProcessor.so")

def main():
    processor = ROOT.RawEventProcessor()

    file = open('/storage/eventSetting.json', 'r')
    settingJson = file.read()
    processor.updateSettings(settingJson)
    processor.setOutputFilePrefix("Event")
    processor.setDir("output_test1")
    processor.setRawEventFilePrefix("RawEvent")
    processor.setRawTreeName("raw_event_tree")
    processor.setRawBranchName("raw_event")
    processor.setEventTreeName("event_tree")
    processor.setEventBranchName("event")
    processor.run()

    time.sleep(100)
    pass
    processor.stop()


if __name__ == "__main__":
    main()