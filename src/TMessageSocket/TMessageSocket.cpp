#include "TMessageSocket.h"
#include "RawEvent.h"
#include "TH1.h"
#include "Channel.h"

ClassImp(TMessageSocket);

TMessageSocket::TMessageSocket(int port, int bufferSize){
    setBufferSize(bufferSize);
    asSender(port);
}
TMessageSocket::TMessageSocket(int port, const char* host, int bufferSize){
    setBufferSize(bufferSize);
    asReceiver(port, host);
}
TMessageSocket::~TMessageSocket(){
    if(buffer != NULL)
        delete buffer;
    status = status_stopping;
    if(mSocketThreadSender!=NULL)mSocketThreadSender->join();
    if(mSocketThreadReceiver!=NULL)mSocketThreadReceiver->join();
}

bool TMessageSocket::put(TObject* obj){
    return buffer->put(obj);
}
TObject* TMessageSocket::get(TClass* cl){
    if(receiver == NULL)return NULL;
    if(!receiver->IsValid())return NULL;
    TMessage *msg;
    int n = receiver->Recv(msg);
    if(n<=0)return NULL;
    return msg->ReadObject(cl);
}

void TMessageSocket::setBufferSize(int size){
    if(buffer != NULL)
        delete buffer;
    buffer = new TMessageBuffer(size);
}
void TMessageSocket::asSender(int port){
    mSocketThreadSender = new thread(&TMessageSocket::senderLoop, this, port);
}
void TMessageSocket::senderLoop(int port){
    status = status_running;
    TServerSocket *server = new TServerSocket(port, kTRUE);
    server->SetOption(kReuseAddr, 1);
    while(status == status_running){
        TSocket *sender = server->Accept();//注意这里可能造成的阻塞
        while(status == status_running && sender->IsValid()){
            TMessage *msg = buffer->get();
            if (msg->What() == kMESS_OBJECT) {
                sender->Send(*msg);
                buffer->getDone();
            }else{
                sleep(0.1);
            }
        }
        sender->Close();
    }
    server->Close();
}

void TMessageSocket::asReceiver(int port, const char* host){
    mSocketThreadReceiver = new thread(&TMessageSocket::receiverLoop, this, port, host);
}
void TMessageSocket::receiverLoop(int port, const char* host){
    status = status_running;
    while(status == status_running){
        receiver = new TSocket(host, port);
        if(receiver->IsValid() == kFALSE){
            sleep(0.1);
            //delete receiver;
            continue;
        }
        receiver->SetOption(kReuseAddr, 1);
        while(status == status_running && receiver->IsValid()){
            sleep(0.1);
        }
    }
    receiver->Close();
}