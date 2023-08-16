#include "TMessageSocket.h"
#include "RawEvent.h"
#include "TH1.h"
#include "Channel.h"

ClassImp(TMessageSocket);

TMessageSocket::TMessageSocket(int _port, int bufferSize){
    setBufferSize(bufferSize);
    port = _port;
    asSender(port);
}
TMessageSocket::TMessageSocket(int _port, const char* _host, int bufferSize){
    port = _port;
    host = _host;
}
TMessageSocket::~TMessageSocket(){
    status = status_stopping;
    if(mSocketThreadSender!=NULL)mSocketThreadSender->join();
    if(receiver!=NULL)receiver->Close();
    if(buffer != NULL) delete buffer;
}

bool TMessageSocket::put(TObject* obj){
    return buffer->put(obj);
}
TMessage* TMessageSocket::getMessage(){
    if(receiver == NULL ){
        cout<<"QA connecting to DP data port ..."<<endl;
        receiver = new TSocket(host.c_str(), port);
        if(receiver->IsValid() == kFALSE){
            receiver = NULL;
            return NULL;
        }
        receiver->SetOption(kReuseAddr, 1);
        receiver->SetOption(kNoBlock, 1);
    }
    if(receiver->IsValid() == kFALSE) {
        receiver->Close();
        receiver = NULL;
        return NULL;
    }
    TMessage *msg;
    int n = receiver->Recv(msg);
    if(n<=0){
        return NULL;
    }
    return msg;
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
    TServerSocket *server = new TServerSocket(port, true);
    // server->SetOption(kNoBlock, 1);
    TMonitor serverMonitor, clientMonitor;
    serverMonitor.Add(server);
    while(status == status_running){
        serverMonitor.ResetInterrupt();
        TSocket* s = serverMonitor.Select(5); 
        if(s == (TSocket *)-1) {
            sleep(1);
            continue;
        }
        TSocket *sender = ((TServerSocket*)s)->Accept();
        if(sender == (TSocket *)-1) continue;
        clientMonitor.Add(sender);
        // sender->SetOption(kNoBlock,1);
        while(status == status_running && sender->IsValid()){
            TMessage *msg = buffer->get();
            if (msg->What() == kMESS_OBJECT) {
                int n = sender->Send(*msg);
                if(n>0)buffer->getDone();
            }else{
                sleep(0.1);
            }
        }
        sender->Close();
    }
    server->Close();
}