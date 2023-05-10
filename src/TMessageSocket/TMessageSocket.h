#ifndef __TMessageSocket_h__
#define __TMessageSocket_h__
#include "TGClient.h"
#include "TObject.h"
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include <atomic>
#include "TString.h"
#include "TMessageBuffer.h"
#include <thread>
#include "TSocket.h"
#include "TServerSocket.h"
#include "TMonitor.h"

using namespace std;

class TMessageSocket:public TObject {
public:
    TMessageSocket(int port, int bufferSize = 100);
    TMessageSocket(int port, const char* host, int bufferSize = 100);
    virtual ~TMessageSocket();
    bool put(TObject* obj);
    TObject* get(TClass* cl);
   
private:
    int port;
    string host;

    TSocket *receiver=NULL;

    TMessageBuffer *buffer=NULL;

    void setBufferSize(int size = 10);

    void asSender(int port);
    thread *mSocketThreadSender=NULL;
    void senderLoop(int port);

    int status_not_started = 0;
    int status_starting = 1;
    int status_running = 2;
    int status_stopping = 3;
    int status_stopped = 4;
    atomic_int status;
   
    ClassDef(TMessageSocket,1)
};

#endif
