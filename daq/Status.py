import json

class Status(object):
    def __init__(self):
        self.dict = dict()

    def update(self, key, value):
        self.dict[key]=value

    def getJSON(self):
        return json.dumps(self.dict)