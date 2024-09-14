# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter SiTCP folder")
project(SiTCP)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(SiTCP SHARED SiTCP.cpp)
add_executable(SiTCPExec main.cpp)

target_link_libraries(SiTCP PUBLIC ROOT::RIO ROOT::Net RawEvent PacketDecoder AutoSocket)
target_link_libraries(SiTCPExec PUBLIC SiTCP)

install(TARGETS SiTCP DESTINATION /usr/lib)
install(TARGETS SiTCPExec DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SiTCP.h DESTINATION include)
