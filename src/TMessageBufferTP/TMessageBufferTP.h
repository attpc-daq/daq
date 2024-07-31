#ifndef __TMessageBufferTP_h__
#define __TMessageBufferTP_h__
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TMessage.h"
#include <atomic>

using namespace std;

template <class objectClass>
class TMessageBufferTP {
public:
    TMessageBufferTP(int size=100);
    virtual ~TMessageBufferTP();
    bool put(objectClass* obj);
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
template <class objectClass>
TMessageBufferTP<objectClass>::TMessageBufferTP(int size){
    buffer = new TMessage*[size];
    for(int i = 0; i < size; i++){
        buffer[i] = new TMessage(kMESS_OBJECT, 1024*1024*20);
    }
    bufferLength = size;
    dataLength = 0;
    head = 0;
    tail = 0;
    msgNull = new TMessage(kMESS_NOTOK);
}
template <class objectClass>
TMessageBufferTP<objectClass>::~TMessageBufferTP(){
    for(int i = 0; i < bufferLength; i++){
        delete buffer[i];
    }
    delete buffer;
}
template <class objectClass>
bool TMessageBufferTP<objectClass>::put(objectClass* obj){
    if(dataLength>=bufferLength){
        return false;
    }
    buffer[tail]->Reset();
    buffer[tail]->SetWhat(kMESS_OBJECT);
    buffer[tail]->WriteObject(obj);
    // cout<<"buffer[tail]->What(): "<<buffer[tail]->What()<<endl;
    
    tail = (tail + 1) % bufferLength;
    dataLength++;
    return true;
}
template <class objectClass>
int TMessageBufferTP<objectClass>::size(){
    return dataLength;
}
template <class objectClass>
TMessage* TMessageBufferTP<objectClass>::get(){
    if(dataLength<=0){
        return msgNull;
    }
    // cout<<"buffer[head]->What(): "<<buffer[head]->What()<<endl;
    return buffer[head];
}
template <class objectClass>
void TMessageBufferTP<objectClass>::getDone(){
    buffer[head]->Reset();
    buffer[head]->SetWhat(kMESS_ANY);
    head = (head + 1) % bufferLength;
    dataLength--;
}

#endif
