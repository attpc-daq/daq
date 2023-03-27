import threading
import time
import asyncio
import websockets
import socket
import queue
import math

from . import Data2ROOT
from . import Utils
from . import Handlers
from . import SiTCP

class Server(threading.Thread):

    def __init__(self, handler, port, host):
        super(Server, self).__init__()
        self._handler = handler
        self._port = port
        self._host = host
        self._state = 'stopped'
    
    def state(self):
        return self._state

    def run(self):
        try:
            self._state = 'running'
            loop = asyncio.new_event_loop()
            asyncio.set_event_loop(loop)
            WSServer = websockets.serve(self._handler, self._host, self._port)
            self._handler.on_server_start(self)
            asyncio.get_event_loop().run_until_complete(WSServer)
            asyncio.get_event_loop().run_forever()
            loop.close()
            self._state = 'stopped'

        except Exception as e:
            Utils.LOGGER.error('error in websockets server loop,%s',e)

def startWebSocketServer(host_ip, host_port):

    _handler = Handler()
    _server = Server(_handler, host_port, host_ip)
    _server.start()

    try:
        while _server.state() == 'running':
            time.sleep(0.1)
    except KeyboardInterrupt:
        _handler.on_shutdown(_server)

    _server.join(5)

class Handler(Handlers.CommandHandler):

    def __init__(self):
        super(Handler, self).__init__()

        self._SiTCP_address="192.168.10.16"
        self._SiTCP_port=4660
        self._receiver_threading = None
        self._SiTCP_sock = None

        self._rootfile_events = 1000
        self._rootfile_prefix = 'Data'
        self._output_directory = '/storage/ATTPC/output'

        self.raw_data_queue = queue.Queue()
        self._root_saver_threading = None

    async def __call__(self, websocket):
        while True:
            try:
                msg = await websocket.recv()
                msg = msg.strip()
                Utils.LOGGER.info("WS Message:%s",msg)
                await self._on_message(websocket,msg)
            except websockets.ConnectionClosedOK:
                Utils.LOGGER.info("Client disconnected")
                break

    async def _on_message(self,websocket,msg):
        super(Handler,self).on_data(websocket,msg)
        for cmd in msg.split(';'):
            args = [
                    val for val in [
                        val.strip() for val in cmd.split(self._seps)
                    ] if val]
            if not args:
                continue
            if not await self.on_command(websocket, args):
                return False

    async def _set_SiTCP_adress(self, address):
        self._SiTCP_address = str(address)

    async def _set_SiTCP_port(self, port):
        self._SiTCP_port = int(port)

    async def _set_rootfile_events(self,events):
        #self._root_saver_threading.set_rootfile_events(events)
        self._rootfile_events = int(events)

    async def _set_rootfile_prefix(self, prefix):
        #self._root_saver_threading.set_rootfile_prefix(prefix)
        self._rootfile_prefix = str(prefix)

    async def _set_output_directory(self, dir):
        #self._root_saver_threading.set_output_directory(dir)
        self._output_directory = str(dir)

    async def _start_root_saver(self):
        self._root_saver_threading = Data2ROOT.ROOTSaver(self.raw_data_queue)
        self._root_saver_threading.set_rootfile_events(self._rootfile_events)
        self._root_saver_threading.set_rootfile_prefix(self._rootfile_prefix)
        self._root_saver_threading.set_output_directory(self._output_directory)
        self._root_saver_threading.start()
    async def stop_SiTCP(self, websocket):
        await self.on_cmd_send_to_SiTCP(websocket,['send_to_SiTCP', '0410203A4050607083'])
        await websocket.send("SiTCP stopped")
    async def clean_SiTCP(self, websocket):
        await self.on_cmd_send_to_SiTCP(websocket,['send_to_SiTCP', '081020384150607083'])
        await websocket.send("SiTCP buffer cleaned")
#______________________________________________________________________________
    
    async def on_cmd_send_to_SiTCP(self, websocket, msg_list):
        if self._receiver_threading is not None:
            message = "receiver is running, can not send message to SiTCP"
            await websocket.send(message)
            return
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self._SiTCP_address, self._SiTCP_port))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            Utils.LOGGER.error("Can not connect to SiTCP port")
            message = "Can not connect to SiTCP port"
            await websocket.send(message)
            return
        msg = bytes.fromhex(str(msg_list[1]))
        sock.send(msg)
        message = str(msg_list[1]) + " done"
        await websocket.send(message)
        sock.close()
        
    async def on_cmd_set_threshold_SiTCP(self, websocket, msg_list):
        #set_threshold_SiTCP FE_ID threshold
        #FE_ID: 0x00-0x1F (hex,两个数值)
        #threshold: 0x0000-0x0FFF (hex,四个数值)
        if self._receiver_threading is not None:
            message = "receiver is running, can not send message to SiTCP"
            await websocket.send(message)
            return
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self._SiTCP_address, self._SiTCP_port))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            Utils.LOGGER.error("Can not connect to SiTCP port")
            message = "Can not connect to SiTCP port"
            await websocket.send(message)
            return
        if(len(msg_list[1])!=2 or len(msg_list[2])!=4):
            message = "FE_ID should be 2 hex number and threshold should be 4 hex number"
            await websocket.send(message)
            return
        FE_id = str(msg_list[1])
        threshold = str(msg_list[2])
        for i in range(0,64):
            #address:1806 data:FE_ID
            cmd = '06102831'+'4'+FE_id[1]+'5'+FE_id[0]+'607083'
            msg = bytes.fromhex(cmd)
            sock.send(msg)
            time.sleep(0.01)
            #address:1808 data:channel
            cmd = '08102831'+'4'+str(hex(i%16))[2]+'5'+str(hex(math.floor(i/16)))[2]+'607083'
            msg = bytes.fromhex(cmd)
            sock.send(msg)
            time.sleep(0.01)
            #address:180A data:threshold
            cmd = '0A102831'+'4'+threshold[3]+'5'+threshold[2]+'6'+threshold[1]+'7'+threshold[0]+'83'
            msg = bytes.fromhex(cmd)
            sock.send(msg)
            time.sleep(0.01)
            #address:180C data:0x0001
            cmd = '0C1028314150607083'
            msg = bytes.fromhex(cmd)
            sock.send(msg)
            time.sleep(0.01)
        message = "FE_ID: "+FE_id+" threshold: "+threshold+" done"
        await websocket.send(message)
        sock.close()
        
    async def on_cmd_read_threshold_file_SiTCP(self, websocket, msg_list):
        #read_threshold_file_SiTCP file_name
        if self._receiver_threading is not None:
            message = "receiver is running, can not send message to SiTCP"
            await websocket.send(message)
            return
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2.0)
        try:
            sock.connect((self._SiTCP_address, self._SiTCP_port))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            Utils.LOGGER.error("Can not connect to SiTCP port")
            message = "Can not connect to SiTCP port"
            await websocket.send(message)
            return
        msg = bytes.fromhex(str(msg_list[1]))
        sock.send(msg)
        message = str(msg_list[1]) + " done"
        await websocket.send(message)
        sock.close()

    async def on_cmd_shutdown(self, websocket, cmd_list):
        await self.on_cmd_close(websocket, cmd_list)
        asyncio.get_event_loop().stop()

    async def on_cmd_run(self, websocket, cmd_list):

        if len(cmd_list) == 1:
            if self._root_saver_threading is None:
                await self._start_root_saver()
            else:
                message = "root saver threading exist"
                await websocket.send(message)

            if self._receiver_threading is None:
                try:
                    self._receiver_threading = SiTCP.Client(self._SiTCP_address, self._SiTCP_port, self.raw_data_queue)
                    self._receiver_threading.start()
                except Exception as exc:
                    Utils.LOGGER.error("NG:Could not start daq. %s", exc)
                    self._receiver_threading.stop()
                    self._receiver_threading = None
            else:
                message = "NG:Run command status mismatch"
                await websocket.send(message)
                Utils.LOGGER.error(message)
        else:
            await websocket.send("NG:Too many arguments")
        return True

    async def on_cmd_stop(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            if self._receiver_threading is not None:
                await websocket.send("daq stopped...")
                self._receiver_threading.stop()
                self._receiver_threading = None
            else:
                Utils.LOGGER.error("stop command status mismatch")
            
            if self._root_saver_threading is not None:
                while not self.raw_data_queue.empty():
                    time.sleep(0.5)
                    Utils.LOGGER.info("Waiting for raw data writing...%s" % self.raw_data_queue.qsize())
                self._root_saver_threading.stop()
                self._root_saver_threading = None
                await websocket.send("raw data saved...")
        else:
            await websocket.send("NG:Too many arguments")
        await websocket.send("daq stopped")
        await self.stop_SiTCP(websocket)
        time.sleep(1)
        await self.clean_SiTCP(websocket)
        return True

    async def on_cmd_set(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            await websocket.send("set what?")
            return True
        if cmd_list[1] == 'SiTCP':
            if cmd_list[2] == 'address':
                await self._set_SiTCP_adress(cmd_list[3])
            if cmd_list[2] == 'port':
                await self._set_SiTCP_port(cmd_list[3])
        if cmd_list[1] == 'rootfile':
            if cmd_list[2] == 'events':
                await self._set_rootfile_events(cmd_list[3])
            if cmd_list[2] == 'prefix':
                await self._set_rootfile_prefix(cmd_list[3])
        if cmd_list[1] == 'output':
            if cmd_list[2] == 'directory':
                await self._set_output_directory(cmd_list[3])
        return True
