#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "EventQA.h"

int main(int argc, char* argv[]){
    EventQA *qa = new EventQA();
    qa->run();
    return EXIT_SUCCESS;
}