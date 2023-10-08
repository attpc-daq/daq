# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter AutoSocket folder")
project(AutoSocket)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(AutoSocket SHARED AutoSocket.cpp)
target_link_libraries(AutoSocket PUBLIC ROOT::RIO ROOT::Net)

install(TARGETS AutoSocket DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/AutoSocket.h DESTINATION include)
