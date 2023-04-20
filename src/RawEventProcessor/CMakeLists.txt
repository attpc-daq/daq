# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter RawEventProcessor folder")
project(RawEventProcessor)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(RawEventProcessor SHARED RawEventProcessor.cpp)
target_link_libraries(RawEventProcessor PUBLIC ROOT::RIO ROOT::Net Event RawEvent TMessageSocket FullEvent)

ROOT_GENERATE_DICTIONARY(RawEventProcessor_dict  RawEventProcessor.h LINKDEF Linkdef.h MODULE RawEventProcessor)

install(TARGETS RawEventProcessor DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/RawEventProcessor.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libRawEventProcessor.rootmap ${PROJECT_BINARY_DIR}/libRawEventProcessor_rdict.pcm DESTINATION lib)
