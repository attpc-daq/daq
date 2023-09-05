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
        cout<<"connecting to server"<<endl;
        receiver = new TSocket(host.c_str(), port);
        receiver->SetOption(kReuseAddr, 1);
        receiver->SetOption(kNoBlock, 1);
        sleep(1);
        char str[32];
        receiver->Recv(str, 32);
        // cout<<strcmp(str, "go")<<endl;
        if(strcmp(str, "go")<0){
            receiver->Close();
            receiver=NULL;
            sleep(1);
            return NULL;
        }
        // clientMonitor.Add(receiver);
    }
    // receiver = clientMonitor.Select(1);
    // if(sc == (TSocket *)-1){
    //     sleep(0.1);
    //     return NULL;
    // }
    if(receiver->IsValid() == kFALSE) {
        // cout<<"no message socket"<<endl;
        receiver->Close();
        receiver = NULL;
        sleep(1);
        return NULL;
    }
    TMessage *msg;
    int n = receiver->Recv(msg);
    // cout<<"get message size "<<n<<endl;
    if(n<=0){
        sleep(1);
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
    // cout<<"Message socket start"<<endl;
    // TMessage *msgNull = new TMessage(kMESS_NOTOK);
    status = status_running;
    TServerSocket *server = new TServerSocket(port, true);
    for(int i =0 ;i<10;i++){
        if(server->IsValid())break;
        // cout<<"can not set up message, try again..."<<endl;
        if(i>8){
            status = status_stopped;
            // cout<<"can not set up message server"<<endl;
        }
        sleep(1);
    }
    server->SetOption(kNoBlock, 1);
    // TMonitor serverMonitor, clientMonitor;
    int qaclient=0;
    serverMonitor.Add(server);
    while(status == status_running){
        serverMonitor.ResetInterrupt();
        TSocket* s = serverMonitor.Select(5); 
        if(s == (TSocket *)-1) {
            sleep(1);
            continue;
        }
        // cout<<"accept QA client..."<<endl;
        TSocket *sender = ((TServerSocket*)s)->Accept();
        // cout<<"QA client acception done"<<endl;
        if(sender == (TSocket *)-1) {
            sleep(1);
            continue;
        }
        qaclient++;
        // cout<<"qa clients "<<qaclient<<endl;
        if(sender->IsValid() == kFALSE){
            sleep(1);
            continue;
        }
        sender->Send("go");
        // clientMonitor.Add(sender);
        // sender->SetOption(kNoBlock,1);
        while(status == status_running){
            if(sender->IsValid() == kFALSE){
                sleep(1);
                break;
                // continue;
            }
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
    // delete msgNull;
}