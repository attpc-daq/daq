# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter EventMaker folder")
project(EventMaker)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(EventMaker SHARED EventMaker.cpp)
target_link_libraries(EventMaker PUBLIC ROOT::RIO ROOT::Net Event RawEvent)

ROOT_GENERATE_DICTIONARY(EventMaker_dict  EventMaker.h LINKDEF Linkdef.h MODULE EventMaker)

install(TARGETS EventMaker DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/EventMaker.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libEventMaker.rootmap ${PROJECT_BINARY_DIR}/libEventMaker_rdict.pcm DESTINATION lib)
