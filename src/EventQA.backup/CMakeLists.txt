# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter EventQA folder")
project(EventSver)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(Event PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(EventQA SHARED EventQA.cpp)
target_link_libraries(EventQA PUBLIC ROOT::RIO ROOT::Net Event RawEvent EventMaker ROOT::Core ROOT::RHTTP)

ROOT_GENERATE_DICTIONARY(EventQA_dict  EventQA.h LINKDEF Linkdef.h MODULE EventQA)

install(TARGETS EventQA DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/EventQA.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libEventQA.rootmap ${PROJECT_BINARY_DIR}/libEventQA_rdict.pcm DESTINATION lib)
