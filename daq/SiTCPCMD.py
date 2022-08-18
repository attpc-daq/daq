from . import Utils
import socket
class TCPClient(object):
    def __init__(self, address, port):
        self._address = address
        self._port = port
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._sock.settimeout(2.0)
        self._connection = False
    
    def connect(self):
        try:
            self._sock.connect((self._address, self._port))
            self._connection = True
        except (socket.gaierror, socket.timeout, OSError) as exc:
            Utils.LOGGER.error("Can not connect to SiTCP command port")
            self._connection = False

    @property
    def connection(self):
        return self._connection;

    def send(self,msg):
        if not self._connection:
            self._sock.send(msg)

    def recv(self,buf):
        return 
