# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter EventConverter folder")
project(EventConverter)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(EventConverter SHARED EventConverter.cpp)
target_link_libraries(EventConverter PUBLIC ROOT::RIO ROOT::Net Event RawEvent)

ROOT_GENERATE_DICTIONARY(EventConverter_dict  EventConverter.h LINKDEF Linkdef.h MODULE EventConverter)

install(TARGETS EventConverter DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/EventConverter.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libEventConverter.rootmap ${PROJECT_BINARY_DIR}/libEventConverter_rdict.pcm DESTINATION lib)
