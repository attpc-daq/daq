import threading
import time
from logging import getLogger, StreamHandler, INFO

LOGGER = getLogger(__name__)
HANDLER = StreamHandler()
HANDLER.setLevel(INFO)
LOGGER.setLevel(INFO)
LOGGER.addHandler(HANDLER)

THREAD_NOT_STARTED = 0
THREAD_STARTING = 1
THREAD_RUNNING = 2
THREAD_STOPPING = 3
THREAD_STOPPED = 4

import queue
Queue = queue.Queue
Empty = queue.Empty
Full = queue.Full

class State(object):
    def __init__(self, initial_state=0):
        self._cond = threading.Condition()
        self._state = initial_state

    def __call__(self):
        return self._state

    @property
    def state(self):
        return self._state

    def transit(self, new_state):
        with self._cond:
            if new_state > self._state:
                self._state = new_state
                self._cond.notify_all()
                return True
            return False

    def wait(self, state, timeout=None):
        with self._cond:
            end = time.time() + (timeout or 0)
            while True:
                if self._state >= state:
                    return True
                if timeout is None:
                    self._cond.wait()
                else:
                    now = time.time()
                    if now > end:
                        return False
                    self._cond.wait(end - now)

def total_seconds(val):
    return (
        (val.days * 86400 + val.seconds) * 10**6 + val.microseconds
    ) * 0.1**6