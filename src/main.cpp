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
#include "DAQHandler.h"
#include "WebSocketServer.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

int main(int argc, char* argv[]){
    cout<<"WebSocketServer PID "<<getpid()<<endl;
    auto const address = net::ip::make_address("0.0.0.0");
    auto port = static_cast<unsigned short>(std::atoi("8000"));
    if(argc >= 2){
        port = static_cast<unsigned short>(std::atoi(argv[1]));
    }
    auto threads = std::max<int>(1, std::atoi("1"));
    if(argc ==3){
        threads = std::max<int>(1, std::atoi(argv[2]));
    }
    static DAQHandler *daqHandler = new DAQHandler();
    net::io_context ioc{threads};
    std::make_shared<listener>(ioc, tcp::endpoint{address, port}, daqHandler)->run();
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back([&ioc]{
            ioc.run();
        });
    ioc.run();
    return EXIT_SUCCESS;
}