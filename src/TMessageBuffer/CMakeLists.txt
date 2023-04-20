# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter TMessageBuffer folder")
project(TMessageBuffer)

find_package(ROOT REQUIRED COMPONENTS RIO Net Hist)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(TMessageBuffer SHARED TMessageBuffer.cpp)
target_link_libraries(TMessageBuffer PUBLIC ROOT::RIO ROOT::Net RawEvent ROOT::Hist)

ROOT_GENERATE_DICTIONARY(TMessageBuffer_dict TMessageBuffer.h LINKDEF Linkdef.h MODULE TMessageBuffer)

install(TARGETS TMessageBuffer DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMessageBuffer.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libTMessageBuffer.rootmap ${PROJECT_BINARY_DIR}/libTMessageBuffer_rdict.pcm DESTINATION lib)
