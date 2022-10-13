import threading
import time
import asyncio
import websockets
import socket
import queue

from . import Data2ROOT
from . import Utils
from . import Handlers
from . import SiTCPData

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

        self._SiTCP_data_threading = None
        self._SiTCP_data_address = "localhost"
        self._SiTCP_data_port = 24242
        self._SiTCP_command_sock = None
        self._SiTCP_command_address = "localhost"
        self._SiTCP_command_port = 9090

        self._rootfile_events = 100
        self._rootfile_prefix = 'Data'
        self._output_directory = './output'

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

    async def _write_SiTCP_register(self, data):
        self._connect_SiTCP_command_port()
        self._SiTCP_command_sock.send(str(data).encode('utf-8'))
        return True

    async def _read_SiTCP_register(self, websocket, cmd_list):
        if self._SiTCP_command_client is None:
            self.connect_SiTCP_command_port()
        
        return True

    def _connect_SiTCP_command_port(self):
        if self._SiTCP_command_sock is not None:
            Utils.LOGGER.info("socket to SiTCP port all ready exist")
            return
        # socket.SOCK_STREAM/TCP, socket.SOCK_DGRAM/UDP
        self._SiTCP_command_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._SiTCP_command_sock.settimeout(2.0)
        try:
            self._SiTCP_command_sock.connect((self._SiTCP_command_address, self._SiTCP_command_port))
        except (socket.gaierror, socket.timeout, OSError) as exc:
            Utils.LOGGER.error("Can not connect to SiTCP command port")
            self._SiTCP_command_sock = None

    async def _set_SiTCP_adress(self, address):
        self._SiTCP_command_address = str(address)

    async def _set_SiTCP_data_port(self, port):
        self._SiTCP_data_port = int(port)
        
    async def _set_SiTCP_command_port(self, port):
        self._SiTCP_command_port = int(port)

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
#______________________________________________________________________________
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

            if self._SiTCP_data_threading is None:
                try:
                    self._SiTCP_data_threading = SiTCPData.TCPClient(self._SiTCP_data_address, self._SiTCP_data_port, self.raw_data_queue)
                    self._SiTCP_data_threading.start()
                except Exception as exc:
                    Utils.LOGGER.error("NG:Could not start daq. %s", exc)
                    self._SiTCP_data_threading.stop()
                    self._SiTCP_data_threading = None
            else:
                message = "NG:Run command status mismatch"
                await websocket.send(message)
                Utils.LOGGER.error(message)
        else:
            await websocket.send("NG:Too many arguments")
        return True

    async def on_cmd_stop(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            if self._SiTCP_data_threading is not None:
                await websocket.send("daq stopped...")
                self._SiTCP_data_threading.stop()
                self._SiTCP_data_threading = None
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
        return True

    async def on_cmd_set(self, websocket, cmd_list):
        if len(cmd_list) == 1:
            await websocket.send("set what?")
            return True
        if cmd_list[1] == 'SiTCP':
            if cmd_list[2] == 'address':
                await self._set_SiTCP_adress(cmd_list[3])
            if cmd_list[2] == 'data':
                if cmd_list[3] == 'port':
                    await self._set_SiTCP_data_port(cmd_list[4])
            if cmd_list[2] == 'command':
                if cmd_list[3] == 'port':
                    await self._set_SiTCP_command_port(cmd_list[4])
            if cmd_list[2] == 'register':
                await self._write_SiTCP_register(cmd_list[3])
        if cmd_list[1] == 'rootfile':
            if cmd_list[2] == 'events':
                await self._set_rootfile_events(cmd_list[3])
            if cmd_list[2] == 'prefix':
                await self._set_rootfile_prefix(cmd_list[3])
        if cmd_list[1] == 'output':
            if cmd_list[2] == 'directory':
                await self._set_output_directory(cmd_list[3])
        return True

    async def on_cmd_get(self, websocket, cmd_list):
        if cmd_list[1] == 'SiTCP':
            if cmd_list[2] == 'register':
                await self._read_SiTCP_register(cmd_list[3])
        return True
