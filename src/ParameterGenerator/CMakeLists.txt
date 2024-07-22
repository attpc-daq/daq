# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter ParameterGenerator folder")
project(ParameterGenerator)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(ParameterGenerator SHARED ParameterGenerator.cpp)
target_link_libraries(ParameterGenerator PUBLIC ROOT::RIO ROOT::Net RawEvent PacketDecoder)

install(TARGETS ParameterGenerator DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ParameterGenerator.h DESTINATION include)
