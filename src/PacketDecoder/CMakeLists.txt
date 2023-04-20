# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter PacketDecoder folder")
project(PacketDecoder)

find_Package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(PacketDecoder SHARED PacketDecoder.cpp)
target_link_libraries(PacketDecoder PUBLIC ROOT::RIO ROOT::Net RawEvent)

ROOT_GENERATE_DICTIONARY(PacketDecoder_dict  PacketDecoder.h LINKDEF Linkdef.h MODULE PacketDecoder)

install(TARGETS PacketDecoder DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/PacketDecoder.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libPacketDecoder.rootmap ${PROJECT_BINARY_DIR}/libPacketDecoder_rdict.pcm DESTINATION lib)
