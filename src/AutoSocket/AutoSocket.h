#ifndef __AutoSocket_h__
#define __AutoSocket_h__
#include "TGClient.h"
#include "TObject.h"
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include <atomic>
#include "TString.h"
#include <thread>
#include "TSocket.h"
#include "TServerSocket.h"
#include "TMonitor.h"

using namespace std;

class AutoSocket:public TObject {
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

    TMonitor serverMonitor;
   
    ClassDef(AutoSocket,1)
};

#endif
