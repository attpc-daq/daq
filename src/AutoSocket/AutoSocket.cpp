#include "AutoSocket.h"

AutoSocket::AutoSocket(int _port){
    port = _port;
    server = new TServerSocket(port, true);
    server->SetOption(kNoBlock, 1);
}
AutoSocket::AutoSocket(int _port, const char* _host){
    port = _port;
    host = _host;
}
AutoSocket::~AutoSocket(){
    if(server!=NULL)server->Close();
    if(sender!=NULL)sender->Close();
    if(receiver!=NULL)receiver->Close();
}

TMessage* AutoSocket::get(){
    if(receiver == NULL ){
        receiver = new TSocket(host.c_str(), port);
    }
    if(receiver->IsValid() == kFALSE) {
        receiver->Close();
        receiver = NULL;
        sleep(1);
        return NULL;
    }
    TMessage *msg;
    int n = receiver->Recv(msg);
    if(n<=0){
        sleep(1);
        delete msg;
        return NULL;
    }
    return msg;
}

bool AutoSocket::send(TMessage* msg){
    if(sender==NULL){
        // cout<<"waiting for connection..."<<endl;
        sender = server->Accept();
        if(sender == (TSocket *)-1) {
            sleep(1);
            sender=NULL;
            return false;
        }
    }
    if(sender->IsValid() == kFALSE){
            sender->Close();
            sender=NULL;
            sleep(1);
            return false;
    }
    int n = sender->Send(*msg);
    if(n>0){
        return true;
    }else{
        sleep(1);
        return false;
    }
}
