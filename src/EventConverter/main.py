# -*- coding:utf-8 -*-
import argparse
import ROOT
import sys

sys.path.append('/usr/local/lib')
ROOT.gInterpreter.Declare('#include "SiTCP.h"')
ROOT.gInterpreter.Declare('#include "RawDataProcessor.h"')
ROOT.gInterpreter.Declare('#include "PacketDecoder.h"')
ROOT.gSystem.Load("libSiTCP.so")
ROOT.gSystem.Load("libRawDataProcessor.so")
ROOT.gSystem.Load("libPacketDecoder.so")

def main():
    processor = ROOT.RawDataProcessor()
    processor.setRawDataDir("abc")
    processor.setRawDataFilePrefix("packet")
    processor.setReadingTempFileName("reading.dat")
    processor.setOutputFileEvents(10)
    processor.setOutputDir("cde")
    processor.setOutputFilePrefix("RawEvent")
    processor.setWritingTempFileName("writing.root")
    print(processor.getRawDataFileList())
    processor.run()
    print(processor.getRawEventFileList())

    pass
    processor.stop()


if __name__ == "__main__":
    main()