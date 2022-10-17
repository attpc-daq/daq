import threading
import os
import time
from tokenize import Double
import ROOT
import numpy as np
import asyncio

class ROOTSaver(threading.Thread):
    def __init__(self, queue, statusHandler):
        super(ROOTSaver, self).__init__()
        self.statusHandler = statusHandler
    
        self._path = "./output"
        self._prefix = 'Data'
        self._name = None
        self._memory = 1*1024*1024*1024 #1Gb
        self._state = 'stopped'
        self._queue = queue
        self._rootfile = None
        self._roottree = None
        self._FE_ID = 0
        self._channel_index = 0
        self._timestamp = 0.
        self._event_id = 0
        self._waveform = None

    @property
    def state(self):
        return self._state
    
    def stop(self):
        self._state = 'stopped'
        self.join(None)

    def timestr(self):
        t = time.localtime()
        timestr = str(t.tm_year)+"{:02}".format(t.tm_mon)+"{:02}".format(t.tm_mday)+"{:02}".format(t.tm_hour)+"{:02}".format(t.tm_min)+"{:02}".format(t.tm_sec)
        return timestr
    
    def init(self):
        self._FE_ID = 0
        self._channel_index = 0
        self._timestamp = 0.
        self._event_id = 0

    def create_rootfile(self):
        if not os.path.exists(self._path):
            os.mkdir(self._path)
        self.update_file_pathname()
        self._rootfile = ROOT.TFile(self._name,'recreate')
        self._roottree = ROOT.TTree('RawData','RawData')
        self._roottree.Branch('FE_ID',self._FE_ID, 'FE_ID/I')
        self._roottree.Branch('channel_index',self._channel_index, 'channel_index/I')
        self._roottree.Branch('timestamp',self._timestamp, 'timestamp/D')
        self._roottree.Branch('event_id',self._event_id, 'event_id/I')
        self._waveform = np.zeros(1024,dtype=int)
        self._roottree.Branch('waveform',self._waveform, 'waveform[1024]/I')

    def close_rootfile(self):
        self._rootfile.cd()
        self._roottree.Write()
        self._rootfile.Close()
        self._rootfile = None
        self._roottree = None

    def run(self):
        self._state = 'running'
        self.init()
        asyncio.run(self.statusHandler.updateStatus('root saver','running'))
        while self._state == 'running':
            #if self._rootfile == None :
            #    self.create_rootfile()
            if not self._queue.empty():
                temp = self._queue.get(True, 0.01)
                length_byte = len(temp)
                count = 0
                time.sleep(1)
                
            #    # find the first header
            #    while(count<length_byte):
            #        if(temp[count]==0x5a and (temp[count+1]&0xE0)==0x40):
            #            self._timestamp = (temp[count+4]<<40) + (temp[count+5]<<32) + (temp[count+6]<<24) + (temp[count+7]<<16) + (temp[count+8]<<8) + temp[count+9]
            #            self._event_id  = (temp[count+10]<<24) + (temp[count+11]<<16) + (temp[count+12]<<8) + temp[count+13]
            #            count += 20   
            #            break
            #        else:
            #            count += 1
            #    # Decode
            #    while(count<length_byte):
            #        if(temp[count]==0x5a):
            #            if((temp[count+1]&0xE0)==0x40):
            #                if(count+13>length_byte):
            #                    count += 20
            #                else:
            #                    self._timestamp = (temp[count+4]<<40) + (temp[count+5]<<32) + (temp[count+6]<<24) + (temp[count+7]<<16) + (temp[count+8]<<8) + temp[count+9]
            #                    self._event_id  = (temp[count+10]<<24) + (temp[count+11]<<16) + (temp[count+12]<<8) + temp[count+13]
            #                    count += 20                         
            #            elif((temp[count+1]&0xE0)==0x20):
            #                count += 12                            
            #            elif((temp[count+1]&0xE0)==0x00):
            #                if(count+7+2048>length_byte):
            #                    count += 2060
            #                else:
            #                    self._FE_ID=temp[count+3]
            #                    self._channel_index=temp[count+4]
            #                    for i in range(0,1024):
            #                        self._waveform[i] = ((temp[count+6+i*2]<<8) + temp[count+7+i*2])&0b0000111111111111
            #                    count += 2060  
            #                    self._roottree.Fill()                             
            #            else:
            #                count += 1
            #        else:
            #            count += 1
            #    
            #    self._memory -= length_byte
            #    if self._memory<=0:
            #        self.close_rootfile()
        asyncio.run(self.statusHandler.updateStatus('root saver','stopped'))

    def set_rootfile_events(self, n): #输入量n以Gb为单位
        self._memory = n*1024*1024*1024
    
    def set_rootfile_prefix(self, prefix):
        self._prefix = prefix

    def set_output_directory(self, dir):
        self._path = dir

    def update_file_pathname(self):
        self._name = os.path.join(self._path, self._prefix+self.timestr()+'.root')
