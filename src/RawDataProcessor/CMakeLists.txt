# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter RawDataProcessor folder")
project(RawDataProcessor)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(RawDataProcessor SHARED RawDataProcessor.cpp)
target_link_libraries(RawDataProcessor PUBLIC ROOT::RIO ROOT::Net RawEvent PacketDecoder TMessageSocket)

ROOT_GENERATE_DICTIONARY(RawDataProcessor_dict RawDataProcessor.h LINKDEF Linkdef.h MODULE RawDataProcessor)

install(TARGETS RawDataProcessor DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/RawDataProcessor.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libRawDataProcessor.rootmap ${PROJECT_BINARY_DIR}/libRawDataProcessor_rdict.pcm DESTINATION lib)
