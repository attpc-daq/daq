import threading
import os
import time
import ROOT
import numpy as np
import copy

class ROOTSaver(threading.Thread):
    def __init__(self, queue):
        super(ROOTSaver, self).__init__()

        self._path = "./output"
        self._prefix = 'Data'
        self._name = None
        self._events = 100
        self._state = 'stopped'
        self._queue = queue
        self._rootfile = None
        self._roottree = None
        
        self._clock_init = 0
        self._clock_final = 0
        self._file_size = 0

        self._event_id = np.array([-1])
        self._timestamp = np.array([-1])
        self._FE_id = np.array([-1])
        self._channel_index = np.array([-1])
        self._waveform = np.zeros((1024),dtype=np.float32)
        self._roottree = ROOT.TTree('RawData','RawData')
        self._roottree.Branch('event_id',self._event_id, 'event_id/L')
        self._roottree.Branch('timestamp',self._timestamp, 'timestamp/L')
        self._roottree.Branch('FE_id',self._FE_id,'FE_id/I')
        self._roottree.Branch('channel_index',self._channel_index,'channel_index/I')
        self._roottree.Branch('waveform',self._waveform,'waveform[1024]/F')

        self._package_count = 0

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

    def create_rootfile(self):
        self._clock_init = time.time()
        self._file_size = 0
        if not os.path.exists(self._path):
            os.mkdir(self._path)
        self.update_file_pathname()
        self._rootfile = ROOT.TFile(self._name,'recreate')

    def close_rootfile(self):
        self._clock_final = time.time()
        delta_t = self._clock_final - self._clock_init
        self._rootfile.cd()
        self._roottree.Write()
        self._rootfile.Close()
        self._rootfile = None
        self._roottree.Reset()
        self._package_count = 0
        print('root file saved:'+self._name)
        print('{} KB/s'.format(self._file_size/delta_t/1024))

    def run(self):
        self._state = 'running'
        firstPackage = True
        dataStream = b''
        while self._state == 'running':
            if self._rootfile == None :
                self.create_rootfile()
                # f = open(self._name+'.dat', "wb")
            if not self._queue.empty():               
                package = self._queue.get(True, 0.01)
                self._file_size += len(package) #unit:byte 8bit
                dataStream = b''.join([dataStream,package])
                # f.write(package)
                self._package_count = self._package_count+1
            else:
                time.sleep(0.1)

            if firstPackage is True:
                length_byte = len(dataStream)
                count = 0
                while count < length_byte:
                    if(dataStream[count]==0x5a and (dataStream[count+1]&0xE0)==0x40):
                        self._timestamp[0] = (dataStream[count+4]<<40) + (dataStream[count+5]<<32) + (dataStream[count+6]<<24) + (dataStream[count+7]<<16) + (dataStream[count+8]<<8) + dataStream[count+9] 
                        self._event_id[0]  = (dataStream[count+10]<<24) + (dataStream[count+11]<<16) + (dataStream[count+12]<<8) + dataStream[count+13]
                        count += 20
                        dataStream = dataStream[count:]
                        firstPackage = False
                        break
                    else:
                        count +=1

            length_byte = len(dataStream)
            count = 0
            while(count+12<length_byte):
                if(dataStream[count]==0x5a):
                    if((dataStream[count+1]&0xE0)==0x40):
                        if(count+13>length_byte):
                            break
                            count = count+20
                        else:
                            self._timestamp[0] = (dataStream[count+4]<<40) + (dataStream[count+5]<<32) + (dataStream[count+6]<<24) + (dataStream[count+7]<<16) + (dataStream[count+8]<<8) + dataStream[count+9]
                            self._event_id[0]  = (dataStream[count+10]<<24) + (dataStream[count+11]<<16) + (dataStream[count+12]<<8) + dataStream[count+13]
                            count = count+20   

                    elif((dataStream[count+1]&0xE0)==0x20):
                        count = count+12

                    elif((dataStream[count+1]&0xE0)==0x00):
                        if(count+7+2048>length_byte):
                            break
                            count = count+2060
                        else:
                            self._FE_id[0] = dataStream[count+3]
                            self._channel_index[0] = dataStream[count+4]
                            for i in range(0,1024):
                                self._waveform[i] = ((dataStream[count+6+i*2]<<8) + dataStream[count+7+i*2])&0b0000111111111111
                            self._roottree.Fill()
                            count = count+2060
                    else:
                        count = count + 1
                else:
                    count = count + 1
            dataStream = dataStream[count:]
            if self._package_count >= self._events:
                self.close_rootfile()
                # f.close()
                        
#--------------------------------------------------------------------
#            if not self._queue.empty():
#                if self._rootfile == None :
#                    self.create_rootfile()
#                    f = open(self._name+'.dat', "wb")
#                datas = self._queue.get(True, 0.01)
#                f.write(datas)
#                self._package_count = self._package_count+1
#                length_byte = len(datas)
#                sequence = b''.join([datas])
#                a=len(sequence)
#                sequence = b''.join([sequence,datas])
#                b=len(sequence)
#                count = 0
#                while(count<length_byte):
#                    if(datas[count]==0x5a and (datas[count+1]&0xE0)==0x40):
#                        self._timestamp[0] = (datas[count+4]<<40) + (datas[count+5]<<32) + (datas[count+6]<<24) + (datas[count+7]<<16) + (datas[count+8]<<8) + datas[count+9] 
#                        self._event_id[0]  = (datas[count+10]<<24) + (datas[count+11]<<16) + (datas[count+12]<<8) + datas[count+13]
#                        count += 20
#                        break
#                    else:
#                        count += 1
#
#                while(count<length_byte):
#                    if(datas[count]==0x5a):
#                        if((datas[count+1]&0xE0)==0x40):
#                            if(count+13>length_byte):
#                                count = count+20
#                            else:
#                                self._timestamp[0] = (datas[count+4]<<40) + (datas[count+5]<<32) + (datas[count+6]<<24) + (datas[count+7]<<16) + (datas[count+8]<<8) + datas[count+9]
#                                self._event_id[0]  = (datas[count+10]<<24) + (datas[count+11]<<16) + (datas[count+12]<<8) + datas[count+13]
#                                count = count+20                         
#                        elif((datas[count+1]&0xE0)==0x20):
#                            count = count+12
#
#                        elif((datas[count+1]&0xE0)==0x00):
#                            if(count+7+2048>length_byte):
#                                count = count+2060
#                            else:
#                                self._FE_id[0] = datas[count+3]
#                                self._channel_index[0] = datas[count+4]
#                                for i in range(0,1024):
#                                    self._waveform[i] = ((datas[count+6+i*2]<<8) + datas[count+7+i*2])&0b0000111111111111
#                                self._roottree.Fill()
#                                count = count+2060                        
#                        else:
#                            count = count+1
#                    else:
#                        count = count+1
#                if self._package_count >= self._events:
#                    self.close_rootfile()
#                    f.close()
#            else:
#                time.sleep(1)


    def set_rootfile_events(self, n):
        self._events = n
    
    def set_rootfile_prefix(self, prefix):
        self._prefix = prefix

    def set_output_directory(self, dir):
        self._path = dir

    def update_file_pathname(self):
        self._name = os.path.join(self._path, self._prefix+self.timestr()+'.root')