# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter SiTCP folder")
project(SiTCP)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(SiTCP SHARED SiTCP.cpp)
target_link_libraries(SiTCP PUBLIC ROOT::RIO ROOT::Net RawEvent PacketDecoder)

ROOT_GENERATE_DICTIONARY(SiTCP_dict SiTCP.h LINKDEF Linkdef.h MODULE SiTCP)

install(TARGETS SiTCP DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/SiTCP.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libSiTCP.rootmap ${PROJECT_BINARY_DIR}/libSiTCP_rdict.pcm DESTINATION lib)
