import threading
import asyncio
import websockets

from . import Utils

class Server(threading.Thread):

    def __init__(self, handler, host, port):
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