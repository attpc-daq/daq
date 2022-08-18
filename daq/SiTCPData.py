from . import Utils
import threading
import select
import socket
import traceback

class TCPClient(threading.Thread):
    def __init__(self, address, port, queue):
        super(TCPClient, self).__init__()
        self._address = address
        self._port = port
        self._raw_data_queue = queue
        self._state = 'stopped'

    def on_data(self, data):
        try:
            self._raw_data_queue.put(data, False)
        except Utils.Full as exc:
            Utils.LOGGER.error("Could not queue raw_data %s", str(exc))
        print('raw data length',len(data))
        return 

    def stop(self):
        self._state = 'stopped'
        self.join(None)

    def run(self):
        self._state = 'running'
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(2.0)
            try:
                sock.connect((self._address, self._port))
            except (socket.gaierror, socket.timeout, OSError) as exc:
                Utils.LOGGER.error("Internal Device Connection Error(%s) %s %s ",
                             str(exc), self._address, self._port)
                return
            read_list = [sock]
            byte_array = bytearray()
            max_buff = 1024 * 1024 * 1024
            while self._state == 'running':
                try:
                    readable, _, _ = select.select(read_list, [], [], 0.01)
                    if sock in readable:
                        byte_array = sock.recv(max_buff)
                        if len(byte_array) > 0 :
                            self.on_data(byte_array)

                except OSError as exc:
                    Utils.LOGGER.error("Internal Daq Process Error (%s)", str(exc))
                    Utils.LOGGER.debug(traceback.format_exc())
                    self._state == 'stopped'
                    break
            sock.close()
        except:
            Utils.LOGGER.error("Error")