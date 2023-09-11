# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter DataProcessor folder")
project(EventSver)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(Event PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(DataProcessor SHARED DataProcessor.cpp)
target_link_libraries(DataProcessor PUBLIC ROOT::RIO ROOT::Net Event RawEvent EventConverter ROOT::Core ROOT::RHTTP AutoSocket)

ROOT_GENERATE_DICTIONARY(DataProcessor_dict  DataProcessor.h LINKDEF Linkdef.h MODULE DataProcessor)

install(TARGETS DataProcessor DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/DataProcessor.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/libDataProcessor.rootmap ${PROJECT_BINARY_DIR}/libDataProcessor_rdict.pcm DESTINATION lib)
