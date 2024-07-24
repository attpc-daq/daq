#include "AutoSocket.h"

AutoSocket::AutoSocket(int _port){
    // constructor for a server, which is for sending data
    port = _port;
    server = new TServerSocket(port, true);
    server->SetOption(kNoBlock, 1);
    buffer = NULL;
    bufferFile = new TBufferFile(TBuffer::kWrite, 1024*1024*10);
}
AutoSocket::AutoSocket(int _port, const char* _host){
    // constructor for a client, which is for receiving data
    // _host is the IP address of the server
    port = _port;
    host = _host;
    buffer = new char[1024*1024*10];// buffer size should be large enough
    bufferFile = new TBufferFile(TBuffer::kRead);
}
AutoSocket::~AutoSocket(){
    if(server!=NULL)server->Close();
    if(sender!=NULL)sender->Close();
    if(receiver!=NULL)receiver->Close();
    if(buffer != NULL)delete [] buffer;
    if(bufferFile != NULL)delete bufferFile;
}

TMessage* AutoSocket::get(){
    // return a TMessage object when receiving data
    // return NULL if no data is received
    if(receiver == NULL ){
        receiver = new TSocket(host.c_str(), port);
        receiver->SetOption(kNoBlock,true);//kNoBlock 防止receiver->Recv(msg)无数据时堵塞
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
TObject * AutoSocket::get(const TClass *cl){
    // return a TObject object when receiving data
    // return NULL if no data is received
    if(receiver == NULL ){
        receiver = new TSocket(host.c_str(), port);
        receiver->SetOption(kNoBlock,true);
    }
    if(receiver->IsValid() == kFALSE) {
        receiver->Close();
        receiver = NULL;
        sleep(1);
        return NULL;
    }
    int size, tmp;
    int n = receiver->Recv(size,tmp);
    if(n<=0){
        sleep(1);
        return NULL;
    }
    n = receiver->RecvRaw(buffer, size);
    if(n<=0){
        sleep(1);
        return NULL;
    }
    bufferFile->Reset();
    bufferFile->SetBuffer(buffer, size, kFALSE);
    return bufferFile->ReadObject(cl);
}

bool AutoSocket::send(TMessage* msg){
    if(sender==NULL){
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

bool AutoSocket::send(TBufferFile* bf){
    if(sender==NULL){
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
    int size = bf->Length();
    int n = sender->Send(size, 0);
    if(n<=0){
        sleep(1);
        return false;
    }
    n = sender->SendRaw(bf->Buffer(), size);
    if(n<=0){
        sleep(1);
        return false;
    }
    return true;
}
bool AutoSocket::send(TObject* obj){
    if(sender==NULL){
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
    bufferFile->Reset();
    bufferFile->WriteObject(obj);

    int size = bufferFile->Length();
    int n = sender->Send(size, 0);
    if(n<=0){
        sleep(1);
        return false;
    }
    n = sender->SendRaw(bufferFile->Buffer(), size);
    if(n<=0){
        sleep(1);
        return false;
    }
    return true;
}
