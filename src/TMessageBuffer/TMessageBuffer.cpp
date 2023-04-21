#include "TMessageBuffer.h"
#include "RawEvent.h"
#include "TH1F.h"
ClassImp(TMessageBuffer);

TMessageBuffer::TMessageBuffer(int size){
    buffer = new TMessage*[size];
    for(int i = 0; i < size; i++){
        buffer[i] = new TMessage();
    }
    bufferLength = size;
    dataLength = 0;
    head = 0;
    tail = 0;
    msgNull = new TMessage(kMESS_NOTOK);
}
TMessageBuffer::~TMessageBuffer(){
    for(int i = 0; i < bufferLength; i++){
        delete buffer[i];
    }
}
bool TMessageBuffer::put(TObject* obj){
    if(dataLength>=bufferLength){
        return false;
    }
    dataLengthLock.lock();
    tailLock.lock();
    buffer[tail]->WriteObject(obj);
    buffer[tail]->SetWhat(kMESS_OBJECT);
    tail = (tail + 1) % bufferLength;
    dataLength++;
    dataLengthLock.unlock();
    tailLock.unlock();
    return true;
}
int TMessageBuffer::size(){
    return dataLength;
}

TMessage* TMessageBuffer::get(){
    if(dataLength<=0){
        return msgNull;
    }
    return buffer[head];
}
void TMessageBuffer::getDone(){
    dataLengthLock.lock();
    headLock.lock();
    buffer[head]->Reset();
    buffer[head]->SetWhat(kMESS_ANY);
    head = (head + 1) % bufferLength;
    dataLength--;
    dataLengthLock.unlock();
    headLock.unlock();
}