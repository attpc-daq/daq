#ifndef __BufferTP_h__
#define __BufferTP_h__
#include <iostream>
#include <string.h>
#include "stdio.h"
#include "TBufferFile.h"
#include <atomic>

using namespace std;

template <class objectClass>
class BufferTP {
public:
    BufferTP(int size=100);
    virtual ~BufferTP();
    bool put(objectClass* obj);
    TBufferFile* get();
    void getDone();
    int size();
private:
    TBufferFile** buffer;

    int bufferLength;
    atomic_int dataLength;
    atomic_int head;
    atomic_int tail;
};
template <class objectClass>
BufferTP<objectClass>::BufferTP(int size){
    buffer = new TBufferFile*[size];
    for(int i = 0; i < size; i++){
        buffer[i] = new TBufferFile(TBuffer::kWrite, 1024*1024*10);
    }
    bufferLength = size;
    dataLength = 0;
    head = 0;
    tail = 0;
}
template <class objectClass>
BufferTP<objectClass>::~BufferTP(){
    for(int i = 0; i < bufferLength; i++){
        delete buffer[i];
    }
    delete buffer;
}
template <class objectClass>
bool BufferTP<objectClass>::put(objectClass* obj){
    if(dataLength>=bufferLength){
        return false;
    }
    buffer[tail]->WriteObject(obj);
    tail = (tail + 1) % bufferLength;
    dataLength++;
    return true;
}
template <class objectClass>
int BufferTP<objectClass>::size(){
    return dataLength;
}
template <class objectClass>
TBufferFile* BufferTP<objectClass>::get(){
    if(dataLength<=0){
        return NULL;
    }
    return buffer[head];
}
template <class objectClass>
void BufferTP<objectClass>::getDone(){
    buffer[head]->Reset();
    head = (head + 1) % bufferLength;
    dataLength--;
}

#endif
