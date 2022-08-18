import json
import types

class Handler(object):

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

    def on_start(self, websocket):
        pass

    def find_delimiter_position(self, byte_data):
        return len(byte_data)

    def on_data(self, websocket, byte_data):
        pass

    def on_idle(self, websocket):
        pass


class TextHandler(Handler):
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

    class _Command(object):
        __slots__ = ["function", "usage_text"]

        def __init__(self):
            self.function = None  # Callable object
            self.usage_text = None  # Usage text of the command.

    def __init__(self):

        super(CommandHandler, self).__init__()

        self._seps = " "
        self._commands = self._find_commands()

    async def on_command(self, websocket, cmd_list):
        assert cmd_list

        cmd = cmd_list[0]
        if cmd in self._commands:
            return await self._commands[cmd].function(websocket, cmd_list)
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

    async def on_cmd_close(self, websocket, _cmd_list):
        await websocket.send('connection will be closed, byebye')
        await websocket.close()
        return False

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

    async def on_cmd_help(self, websocket, _cmd_list):
        await websocket.send('connection will be closed, byebye')
        await websocket.close()
        return False