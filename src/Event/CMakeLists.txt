# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter Event folder")
project(Event)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Event SHARED Event.cpp Pad.cpp)
target_link_libraries(Event PUBLIC ROOT::RIO ROOT::Net)

ROOT_GENERATE_DICTIONARY(Event_dict Pad.h Event.h LINKDEF Linkdef.h MODULE Event)

install(TARGETS Event DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Pad.h ${CMAKE_CURRENT_SOURCE_DIR}/Event.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libEvent.rootmap ${PROJECT_BINARY_DIR}/libEvent_rdict.pcm DESTINATION lib)