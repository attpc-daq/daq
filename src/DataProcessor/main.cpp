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
#include "DataProcessor.h"

int main(int argc, char* argv[]){
    DataProcessor *dp = new DataProcessor(std::atoi(argv[1]));
    dp->run();
    return EXIT_SUCCESS;
}