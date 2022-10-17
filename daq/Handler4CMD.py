import time
import asyncio
import websockets
import socket
import queue

from . import Data2ROOT
from . import Utils
from . import Handlers
from . import SiTCPData

class Handler(Handlers.CommandHandler):

    def __init__(self, statusHandler):
        super(Handler, self).__init__()

        self.clients = set()

        self._SiTCP_daq_threading = None

        self._SiTCP_sock = None
        self._SiTCP_port = 9090
        self._SiTCP_address = "localhost"

        self._rootfile_events = 100
        self._rootfile_prefix = 'Data'
        self._output_directory = './output'

        self.raw_data_queue = queue.Queue()
        self._root_saver_threading = None

        self.statusHandler = statusHandler

    async def __call__(self, websocket):
        self.clients.add(websocket)
        await self.statusHandler.updateStatus('Command clients',len(self.clients))
        while True:
            try:
                msg = await websocket.recv()
                msg = msg.strip()
                Utils.LOGGER.info("WS Message:%s",msg)
                await self._on_message(websocket,msg)
            except websockets.ConnectionClosedOK:
                self.clients.remove(websocket)
                await self.statusHandler.updateStatus('Command clients',len(self.clients))
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
    #____________________________________________________________
    # _ functions
    #____________________________________________________________
    def _connect_SiTCP(self):
        if self._SiTCP_sock is not None:
            Utils.LOGGER.info("socket to SiTCP all ready exist")
            return
        self._SiTCP_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._SiTCP_sock.settimeout(2.0)
        try:
            self._SiTCP_sock.connect((self._SiTCP_address, self._SiTCP_port))
            self.statusHandler.updateStatus('SiTCP socket','connected')
        except (socket.gaierror, socket.timeout, OSError) as exc:
            Utils.LOGGER.error("Can not connect to SiTCP")
            self._SiTCP_sock = None
            self.statusHandler.updateStatus('SiTCP socket','disconnected')

    async def _set_SiTCP_adress(self, address):
        self._SiTCP_address = str(address)
        self.statusHandler.updateStatus('SiTCP address',str(address))

    async def _set_SiTCP_port(self, port):
        self._SiTCP_port = int(port)
        self.statusHandler.updateStatus('SiTCP port', port)

    async def _set_rootfile_events(self,events):
        #self._root_saver_threading.set_rootfile_events(events)
        self._rootfile_events = int(events)
        self.statusHandler.updateStatus('events per file', str(events))

    async def _set_rootfile_prefix(self, prefix):
        #self._root_saver_threading.set_rootfile_prefix(prefix)
        self._rootfile_prefix = str(prefix)
        self.statusHandler.updateStatus('rootfile prefix', str(prefix))

    async def _set_output_directory(self, dir):
        #self._root_saver_threading.set_output_directory(dir)
        self._output_directory = str(dir)
        self.statusHandler.updateStatus('Output dir', str(dir))

    async def _start_root_saver(self, websocket):
        if self._root_saver_threading is not None:
            await websocket.send("root saver threading exist")
            return
        self._root_saver_threading = Data2ROOT.ROOTSaver(self.raw_data_queue,self.statusHandler)
        self._root_saver_threading.set_rootfile_events(self._rootfile_events)
        self._root_saver_threading.set_rootfile_prefix(self._rootfile_prefix)
        self._root_saver_threading.set_output_directory(self._output_directory)
        self._root_saver_threading.start()
        await self.statusHandler.updateStatus('root saver','running')

    def _stop_root_saver(self):
        if self._root_saver_threading is None:
            return
        while not self.raw_data_queue.empty():
            time.sleep(0.5)
        self._root_saver_threading.stop()
        self._root_saver_threading = None
        return 

    async def _start_daq(self, websocket):
        if self._SiTCP_daq_threading is None:
            try:
                #await self.statusHandler.updateStatus('raw data queue size', self.raw_data_queue.qsize())
                self._SiTCP_daq_threading = SiTCPData.TCPClient(self._SiTCP_address, self._SiTCP_port, self.raw_data_queue, self.statusHandler)
                self._SiTCP_daq_threading.start()
                #await self.statusHandler.updateStatus('daq thread','running')
            except Exception as exc:
                Utils.LOGGER.error("NG:Could not start daq. %s", exc)
                self._SiTCP_daq_threading.stop()
                self._SiTCP_daq_threading = None
                #await self.statusHandler.updateStatus('daq thread','stopped')
   
    def _stop_daq(self):
        if self._SiTCP_daq_threading is None:
            return
        self._SiTCP_daq_threading.stop()
        self._SiTCP_daq_threading = None
        return
    async def _send_sitcp(self,msg):
        if self._SiTCP_sock is None:
            await self._connect_SiTCP()
        await self._SiTCP_sock.send(msg)
        return
    #_____________________________________________________
    # on_cmd_xxx
    #_____________________________________________________
    async def on_cmd_shutdown(self, websocket, cmd_list):
        await self.on_cmd_close(websocket, cmd_list)
        asyncio.get_event_loop().stop()

    async def on_cmd_start(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            await self._start_root_saver(websocket)
            await self._start_daq(websocket)
            return       
        if cmd_list[1] == 'rootsaver':
            await self._start_root_saver(websocket)
        if cmd_list[1] == 'daq':
            await self._start_daq(websocket)
        return

    async def on_cmd_stop(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            self._stop_daq()
            self._stop_root_saver()
        if cmd_list[1] == 'rootsaver':
            self._stop_root_saver()
        if cmd_list[1] == 'daq':
            self._stop_daq()
        #await websocket.send("daq stopped")
        return

    async def on_cmd_SiTCP(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            await websocket.send("format error! \' SiTCP xxxx \'")
            return 
        await self._send_sitcp(cmd_list[1])
        return

    async def on_cmd_set(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            await websocket.send("set what?")
            return True
        if cmd_list[1] == 'SiTCP':
            if cmd_list[2] == 'address':
                await self._set_SiTCP_adress(cmd_list[3])
            if cmd_list[2] == 'port':
                await self._set_SiTCP_data_port(cmd_list[3])

        if cmd_list[1] == 'rootfile':
            if cmd_list[2] == 'events':
                await self._set_rootfile_events(cmd_list[3])
            if cmd_list[2] == 'prefix':
                await self._set_rootfile_prefix(cmd_list[3])
        if cmd_list[1] == 'output':
            if cmd_list[2] == 'directory':
                await self._set_output_directory(cmd_list[3])
        return