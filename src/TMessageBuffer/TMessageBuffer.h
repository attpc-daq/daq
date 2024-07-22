#ifndef __TMessageBuffer_h__
#define __TMessageBuffer_h__

#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include <atomic>

using namespace std;

class TMessageBuffer {
public:
    TMessageBuffer(int size=100);
    virtual ~TMessageBuffer();
    bool put(TObject* obj);
    TMessage* get();
    void getDone();
    int size();
    TMessage *msgNull;
private:
    TMessage** buffer;

    int bufferLength;
    atomic_int dataLength;
    atomic_int head;
    atomic_int tail;

};

#endif
