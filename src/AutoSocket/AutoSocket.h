#ifndef __AutoSocket_h__
#define __AutoSocket_h__
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include "TSocket.h"
#include "TServerSocket.h"
#include "TBufferFile.h"
#include "TBuffer.h"

using namespace std;

class AutoSocket {
public:
    AutoSocket(int port);
    AutoSocket(int port, const char* host);
    virtual ~AutoSocket();
    bool send(TObject* obj);
    bool send(TMessage* msg);
    bool send(TBufferFile* bf);
    TMessage* get();
    TObject * get(const TClass *cl);
    
private:
    TBufferFile *bufferFile;
    char* buffer;
    int port;
    string host;
    TSocket *receiver=NULL;
    TSocket *sender=NULL;
    TServerSocket *server=NULL;
};

#endif
