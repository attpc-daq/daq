# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter RawEvent folder")
project(RawEvent)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(RawEvent SHARED RawEvent.cpp Channel.cpp)
target_link_libraries(RawEvent PUBLIC ROOT::RIO ROOT::Net)

ROOT_GENERATE_DICTIONARY(RawEvent_dict  Channel.h RawEvent.h LINKDEF Linkdef.h MODULE RawEvent)

install(TARGETS RawEvent DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Channel.h ${CMAKE_CURRENT_SOURCE_DIR}/RawEvent.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libRawEvent.rootmap ${PROJECT_BINARY_DIR}/libRawEvent_rdict.pcm DESTINATION lib)
