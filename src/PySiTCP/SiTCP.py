import GUISocket.Utils as Utils
import select
import socket
import multiprocessing
import time
import os

class Process(multiprocessing.Process):
    def __init__(self):
        super(Process, self).__init__()

        self._state = Utils.State()
        self._connectionState = False

        self.pipe_parent, self.pipe_child = multiprocessing.Pipe()

        self.dir = "./output/"
        self.socketBufferSize = 1024*1024
        self.fileMaxSize = 100*1024*1024
        self.filePrefix = "packet"
        self.ip = None
        self.port = 4660
        self.file = None
        self.fileID = -1
        self.read_list = []

    # @property
    def state(self):
        self.pipe_parent.send("state")

    def rsp_state(self):
        self.pipe_child.send(self._state.state)

    def setDir(self,dir):
        self.pipe_parent.send("setDir")
        self.pipe_parent.send(dir)

    def rsp_setDir(self):
        self.dir = self.pipe_child.recv()
        if not self.dir.endswith("/"):
            self.dir = self.dir + "/"

        if not os.path.exists(self.dir):
            os.mkdir(self.dir)
        
        self.pipe_child.send("done")

    def updateFileID(self):
        self.fileID = -1
        for filename in os.listdir(self.dir):
            if filename.startswith(self.filePrefix) and filename.endswith(".dat"):
                id = int(filename[len(self.filePrefix):-len(".dat")])
                if id > self.fileID:
                    self.fileID = id
        
        self.fileID += 1

    def createFile(self):
        self.file = open(self.dir+"writing.dat","wb")
    
    def closeFile(self):
        self.file.close()
        self.updateFileID()
        os.rename(self.dir+"writing.dat", self.dir+self.filePrefix+str(self.fileID)+".dat")

    def connectToDevice(self, ip, port):
        self.pipe_parent.send("connectToDevice")
        self.pipe_parent.send(ip)
        self.pipe_parent.send(port)

    def rsp_connectToDevice(self):
        self.ip = self.pipe_child.recv()
        self.port = int(self.pipe_child.recv())
        if self.connect2SiTCP():
            self.pipe_child.send("done")
        else:
            self.pipe_child.send("fail")

    def setSocketBufferSize(self, size):
        self.pipe_parent.send("setSocketBufferSize")
        self.pipe_parent.send(size)

    def rsp_setSocketBufferSize(self):
        self.socketBufferSize = int(self.pipe_child.recv())
        self.pipe_child.send("done")

    def setFileMaxSize(self, size):
        self.pipe_parent.send("setFileMaxSize")
        self.pipe_parent.send(size)
    
    def rsp_setFileMaxSize(self):
        self.fileMaxSize = int(self.pipe_child.recv())
        self.pipe_child.send("done")

    def setFilePrefix(self, prefix):
        self.pipe_parent.send("setFilePrefix")
        self.pipe_parent.send(prefix)

    def rsp_setFilePrefix(self):
        msg = self.pipe_child.recv()
        self.filePrefix = msg
        self.pipe_child.send("done")

    def sendToDevice(self,msg):
        self.pipe_parent.send("sendToDevice")
        self.pipe_parent.send(msg)

    def rsp_sendToDevice(self):
        while self.pipe_child.poll():
            msglist = self.pipe_child.recv()
            self.connect2SiTCP()
            count = 0
            l = len(msglist)
            for msg in msglist:
                try:
                    time.sleep(0.01)
                    self._sock.send(msg)
                    count += 1
                    if count%100 == 0:
                        self.pipe_child.send(str(int(count*100/l))+"%")
                except:
                    raise Exception('SiTCP send failed')         
        
        self.pipe_child.send("done")

    def response(self):
        while self.pipe_child.poll():
            msg = self.pipe_child.recv()
            if msg == "sendToDevice":
                self.rsp_sendToDevice()
            elif msg == "setFilePrefix":
                self.rsp_setFilePrefix()
            elif msg == "setFileMaxSize":
                self.rsp_setFileMaxSize()
            elif msg == "setSocketBufferSize":
                self.rsp_setSocketBufferSize()
            elif msg == "connectToDevice":
                self.rsp_connectToDevice()
            elif msg == "setDir":
                self.rsp_setDir()


    def wait(self):
        msg = self.pipe_parent.recv()
        return msg

    def start(self):
        super(Process, self).start()
        self._state.transit(Utils.PROCESS_STARTING)
        self._state.wait(Utils.PROCESS_RUNNING)
    
    def stop(self):
        if self._state() == Utils.PROCESS_RUNNING:
            self._state.transit(Utils.PROCESS_STOPPING)
            self._state.wait(Utils.PROCESS_STOPPED)
            self.terminate()
            self.join()
            self.close()

    def connect2SiTCP(self):
        if self._connectionState is True:
            return True
        if self.ip == None:
            return False
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.read_list = [self._sock]
        self._sock.settimeout(2)
        self._connectionState = True
        try:
            self._sock.connect((self.ip, self.port))
            self._connectionState = True
            return True
        except (socket.gaierror, socket.timeout, OSError) as exc:
            self._connectionState = False
            return False
            # raise Exception('SiTCP connection error')

    def run(self):
        try:
            print("data acquisition loop start")
            self.connect2SiTCP()
            fileSize = 0
            self.createFile()
            byte_array = bytearray()
            self._state.transit(Utils.PROCESS_RUNNING)
            while self._state() == Utils.PROCESS_RUNNING:
                    
                    while self.pipe_child.poll():
                        self.response()

                    if not self.connect2SiTCP():
                        continue;
                    try:
                        readable, _, _ = select.select(self.read_list, [], [], 0.01)
                        if self._sock in readable:
                            byte_array = self._sock.recv(self.socketBufferSize)

                            if len(byte_array) <= 0 :
                                print("get bytes ",len(byte_array))
                                continue

                            fileSize += len(byte_array)
                            self.file.write(byte_array)
                            if fileSize > self.fileMaxSize:
                                self.closeFile()
                                self.createFile()
                                fileSize = 0
 

                    except OSError as exc:
                        raise Exception('SiTCP loss connection when running')

            self.closeFile()
            self._state.transit(Utils.PROCESS_STOPPING)
            self._sock.close()
            self._connectionState = False


        finally:
            self._state.transit(Utils.PROCESS_STOPPED)
