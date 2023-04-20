#ifndef __TMessageBuffer_h__
#define __TMessageBuffer_h__
#include "TGClient.h"
#include "TObject.h"
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include <atomic>
#include "RawEvent.h"
#include <mutex>

using namespace std;

class TMessageBuffer:public TObject {
public:
    TMessageBuffer(int size);
    virtual ~TMessageBuffer();
    bool put(TObject* obj);
    TMessage* get();
    void getDone();
    int size();
    TMessage *msgNull;
private:
    TMessage** buffer;
    mutex headLock;
    mutex tailLock;
    mutex dataLengthLock;

    int bufferLength;
    atomic_int dataLength;
    atomic_int head;
    atomic_int tail;

    ClassDef(TMessageBuffer,1)
};

#endif
