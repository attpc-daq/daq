#ifndef __AutoSocket_h__
#define __AutoSocket_h__
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/types.h>
// #include <sys/wait.h>
#include <thread>
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "TMessage.h"
#include "TBufferFile.h"
#include "TBuffer.h"
#include "RawEvent.h"


using namespace std;

class AutoSocket {
public:
    AutoSocket(int port);
    AutoSocket(int port, const char* host);
    virtual ~AutoSocket();
    bool send(TBufferFile* bf, bool debug=false);
    bool send(RawEvent* rawEvent, bool debug=false);
    TObject * get(const TClass *cl, bool debug=false);
    
private:
    bool checkConnection(bool debug = false);
    void disconnect();
    bool isSender;
    bool isReceiver;
    bool connection;
    TBufferFile *bufferFile;
    char* buffer;
    char* ACKBuffer;
    int size1,size2,size3;
    int port;
    string host;
    int receiver_fd;
    int sender_fd;
    int server_fd;
    struct sockaddr_in server_addr;
};

#endif
