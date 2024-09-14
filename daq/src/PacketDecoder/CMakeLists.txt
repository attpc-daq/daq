# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter PacketDecoder folder")
project(PacketDecoder)

find_Package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(PacketDecoder SHARED PacketDecoder.cpp)
target_link_libraries(PacketDecoder PUBLIC ROOT::RIO ROOT::Net RawEvent)

install(TARGETS PacketDecoder DESTINATION /usr/lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/PacketDecoder.h DESTINATION include)
