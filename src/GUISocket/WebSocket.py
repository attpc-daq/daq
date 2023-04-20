import threading
import asyncio
import websockets

from . import Utils


class Server(threading.Thread):
    '''
    websocket 线程, 提供GUI后端服务.
    '''
    def __init__(self, handler, port, host):
        super(Server, self).__init__()
        self._handler = handler
        self._port = port
        self._host = host
        self._state = Utils.State()
    
    @property
    def state(self):
        return self._state

    def run(self):
        try:
            loop = asyncio.new_event_loop()
            self._handler.loop = loop
            asyncio.set_event_loop(loop)
            WSServer = websockets.serve(self._handler, self._host, self._port)
            asyncio.get_event_loop().run_until_complete(WSServer)
            self._state.transit(Utils.PROCESS_RUNNING)
            asyncio.get_event_loop().run_forever()
            self._state.transit(Utils.PROCESS_STOPPING)
            loop.close()

        except Exception as e:
            print('error in websockets server loop,%s',e)
        finally:
            self._state.transit(Utils.PROCESS_STOPPED)
