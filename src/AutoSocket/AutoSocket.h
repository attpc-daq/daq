#ifndef __AutoSocket_h__
#define __AutoSocket_h__
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include "TSocket.h"
#include "TServerSocket.h"

using namespace std;

class AutoSocket {
public:
    AutoSocket(int port);
    AutoSocket(int port, const char* host);
    virtual ~AutoSocket();
    bool send(TMessage* msg);
    TMessage* get();
   
private:
    int port;
    string host;
    TSocket *receiver=NULL;
    TSocket *sender=NULL;
    TServerSocket *server=NULL;
};

#endif
