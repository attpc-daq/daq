# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "OnlineQA folder")
project(OnlineQA)

find_package(ROOT REQUIRED COMPONENTS RIO Net Core RHTTP Physics)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(OnlineQA SHARED OnlineQA.cpp)
target_link_libraries(OnlineQA PUBLIC ROOT::RIO ROOT::Net AutoSocket Event RawEvent EventConverter ROOT::Core ROOT::RHTTP ROOT::Physics ROOT::Tree ROOT::Graf)

add_executable(OnlineQAExec main.cpp)
target_link_libraries(OnlineQAExec PUBLIC OnlineQA)

install(TARGETS OnlineQA DESTINATION /usr/lib)
#install(TARGETS OnlineQA DESTINATION lib)
install(TARGETS OnlineQAExec DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/OnlineQA.h DESTINATION include)
