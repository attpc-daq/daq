# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter TMessageSocket folder")
project(TMessageSocket)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(TMessageSocket SHARED TMessageSocket.cpp)
target_link_libraries(TMessageSocket PUBLIC ROOT::RIO ROOT::Net TMessageBuffer RawEvent)

install(TARGETS TMessageSocket DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMessageSocket.h DESTINATION include)
