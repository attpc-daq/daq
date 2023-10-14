'''
此文件定义全局变量，和功能性小工具
'''
import threading
import time
import logging
import json
import types
import os
import queue
import asyncio
from io import StringIO
import multiprocessing
import websockets
import secrets

PROCESS_NOT_STARTED = 0
PROCESS_STARTING = 1
PROCESS_RUNNING = 2
PROCESS_STOPPING = 3
PROCESS_STOPPED = 4


class State(object):
    '''
    方便操作进程状态
    '''
    def __init__(self, initial_state=0):
        self._state = multiprocessing.Value('i',initial_state)

    def __call__(self):
        return self._state.value

    @property
    def state(self):
        return self._state.value

    def transit(self, new_state):
            if new_state > self._state.value:
                self._state.value = new_state
                return True
            return False

    def wait(self, state):
            
            while True:
                if self._state.value >= state:
                    break
                time.sleep(0.1)
    
    def string(self):
        if self._state.value == 0:
            return 'NOT_STARTED'
        if self._state.value == 1:
            return 'STARTING'
        if self._state.value == 2:
            return 'RUNNING'
        if self._state.value == 3:
            return 'STOPPING'
        if self._state.value == 4:
            return 'STOPPED'


class Handler(object):
    '''
    websocket handler的父类, 用于响应GUI的指令
    '''
    def __init__(self):
        self._exit = False

    def on_server_start(self, server):
        pass

    def set_exit(self):
        self._exit = True

    def is_exit(self):
        return self._exit

    def on_shutdown(self, server):
        pass

    def on_start(self):
        pass

    def find_delimiter_position(self, byte_data):
        return len(byte_data)

    def on_idle(self, websocket):
        pass


class TextHandler(Handler):
    '''
    websocket handler的父类, 用于响应GUI的指令
    '''
    def __init__(self):
        super(TextHandler, self).__init__()

        self._linesep = "\r\n"

    def find_delimiter_position(self, byte_data):
        for delimiter, delimiter_str in ((b"\r\n", "\r\n"),
                                         (b"\n", "\n"),
                                         (b"\r", "\r")):
            pos = byte_data.find(delimiter)
            if pos >= 0:
                self._linesep = delimiter_str
                return pos + len(delimiter)
        return -1

    @property
    def linesep(self):
        return self._linesep

class CommandHandler(TextHandler):
    '''
    websocket handler的父类, 用于响应GUI的指令
    '''
    class _Command(object):
        __slots__ = ["function", "usage_text"]

        def __init__(self):
            self.function = None  # Callable object
            self.usage_text = None  # Usage text of the command.

    def __init__(self):

        super(CommandHandler, self).__init__()

        self._seps = " "
        self._commands = self._find_commands()

    async def on_command(self, websocket, cmd_list, client_key):
        assert cmd_list

        cmd = cmd_list[0]
        if cmd in self._commands:
            return await self._commands[cmd].function(websocket, cmd_list, client_key)
        else:
            await websocket.send("NG:Unknown command [%s]" % cmd)
            return True

    def _find_commands(self):
        result = {}

        prefix = "on_cmd_"
        prefix_len = len(prefix)
        funcs = [getattr(self, a, None) for a in dir(self)
                 if isinstance(getattr(self, a, None), types.MethodType)]
        for func in funcs:
            func_name = func.__func__.__name__
            if len(func_name) > prefix_len and func_name[:prefix_len] == prefix:
                func_name = func_name[prefix_len:]

                cmd = self._Command()
                cmd.function = func

                doc = (func.__func__.__doc__ or "").strip()
                for usage_keyword in (":usage:", ":usage", "@usage:", "@usage"):
                    usage_pos = doc.find(usage_keyword)
                    if usage_pos >= 0:
                        cmd.usage_text = doc[usage_pos + len(usage_keyword):].strip()
                        break

                if not cmd.usage_text:
                    cmd.usage_text = "%s: No usage for command." % func_name

                result[func_name] = cmd

        return result


    @staticmethod
    def create_stat_dict(stat_list):
        return dict([val.split("=", 1) for val in stat_list])

    def create_stat_list(self):
        return []

    async def on_cmd_stat(self, websocket, cmd_list):
        json_output = False

        if len(cmd_list) > 1:
            if len(cmd_list) == 2 and cmd_list[1] == "j":
                json_output = True
            else:
                await websocket.send("NG:Unknown argument %s" % (cmd_list[1:]))
                return True

        # Create stat list
        stat_list = []
        stat_list.extend(self.create_stat_list())

        # Output
        if json_output:
            await websocket.send(json.JSONEncoder().encode(
                                self.create_stat_dict(stat_list)))
        else:
            for stat in stat_list:
                await websocket.send(stat)

        return True

    async def on_cmd_help(self, websocket):
        return False

class WebsocketHander(CommandHandler):
    '''
    websocket handler
    '''
    def __init__(self):
        super(WebsocketHander,self).__init__()
        self._ClientList = {}
        self._MonitorList = {}

    async def __call__(self, websocket):
        '''
        websocket 调用入口
        '''
        client_key = secrets.token_urlsafe(12)
        self._ClientList[client_key] = websocket

        while True:
            try:
                msg = await websocket.recv()
                msg = msg.strip()
                await self._on_message(websocket,msg,client_key)
            except websockets.ConnectionClosedOK:
                del self._ClientList[client_key]
                if client_key in self._MonitorList:
                    del self._MonitorList[client_key]
                break

    async def _on_message(self,websocket,msg,client_key):
        for cmd in msg.split(';'):
            args = [
                    val for val in [
                        val.strip() for val in cmd.split(self._seps)
                    ] if val]
            if not args:
                continue
            if not await self.on_command(websocket, args, client_key):
                return False
    
    async def notifymonitor(self,msg):
        #for websocket in self._ClientList.values():
        '''
        仅注册为monitor的client会收到消息
        '''
        for websocket in self._MonitorList.values():
            await websocket.send(msg)

    async def on_cmd_register(self, websocket, cmd_list, client_key):
        self._MonitorList[client_key] = websocket

    async def on_cmd_shutdown(self, websocket, cmd_list, client_key):
        await websocket.close()
        MonitorList = self._MonitorList.copy()
        for ws in MonitorList.values():
            await ws.close()
        
        asyncio.get_event_loop().stop()


def timeString():
    '''
    全局函数, 不存在线程安全问题, 返回调用时的时间字符串
    '''
    t = time.localtime()
    timestr = str(t.tm_year)+"{:02}".format(t.tm_mon)+"{:02}".format(t.tm_mday)+"{:02}".format(t.tm_hour)+"{:02}".format(t.tm_min)+"{:02}".format(t.tm_sec)
    return timestr

class logger(object):
    '''
    自定义logger工具, 方便操作log文件的创建和保存
    调用 logger.stream.getvalue()可以获取当前handler记录的log内容
    '''
    def __init__(self, loggerName, path='./', prefix='Log'):
        self._logger = logging.getLogger(loggerName)
        self._logger.setLevel(logging.DEBUG)
        self._path = path
        self._prefix = prefix
        self._fileHandler = None
        self._streamHandler = None
        self.filePathName = None

    def new(self):
        if self._fileHandler != None:
            self._logger.removeHandler(self._fileHandler)
        self._fileHandler = self._makeFileHandler()
        self._logger.addHandler(self._fileHandler)

        if self._streamHandler != None:
            self._logger.removeHandler(self._streamHandler)
        self._streamHandler = self._makeStreamHandler()
        self._logger.addHandler(self._streamHandler)

        self._logger.info('New Created')

    def setPath(self, path):
        self._path = path

    def setPrefix(self, _prefix):
        self._prefix = _prefix

    def _makeFileHandler(self):
        self.filePathName = os.path.join(self._path, self._prefix+timeString()+'.txt')
        handler = logging.FileHandler(self.filePathName, mode='a')
        handler.setLevel(logging.DEBUG)
        handler.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
        handler.flush()
        return handler

    def _makeStreamHandler(self):
        self.stream = StringIO()
        handler = logging.StreamHandler(self.stream)
        handler.setLevel(logging.DEBUG)
        handler.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
        handler.flush()
        return handler

    def info(self, *args, **kwargs):
        self._logger.info(*args, **kwargs)

    def error(self, *args, **kwargs):
        self._logger.error(*args, **kwargs)

    def debug(self, *args, **kwargs):
        self._logger.debug(*args, **kwargs)

LOGGER = logger(__name__) #logger 全局对象
