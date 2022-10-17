import asyncio
import functools
import os
import sys
import time

import websockets
from . import Handlers
from . import Status
import json


class Handler(Handlers.CommandHandler):
    def __init__(self):
        super(Handler, self).__init__()
        self.clients = set()
        self.status = Status.Status()

    async def __call__(self, websocket):
        self.clients.add(websocket)
        await self.updateStatus('monitor clients',len(self.clients))
        await websocket.send(self.status.getJSON())
        try:
            await websocket.wait_closed()
        finally:
            self.clients.remove(websocket)
            await self.updateStatus('monitor clients',len(self.clients))

    async def updateStatus(self, key, value):
        self.status.update(key,value)
        websockets.broadcast(self.clients,str({key:value}))

