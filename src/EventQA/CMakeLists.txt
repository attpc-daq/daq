# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message(STATUS "Enter EventQA folder")
project(EventSver)

find_package(ROOT REQUIRED COMPONENTS RIO Net Core RHTTP Physics)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(EventQA SHARED EventQA.cpp)
target_link_libraries(EventQA PUBLIC ROOT::RIO ROOT::Net AutoSocket Event RawEvent EventConverter ROOT::Core ROOT::RHTTP ROOT::Physics ROOT::Tree ROOT::Graf)

add_executable(EventQAExec main.cpp)
target_link_libraries(EventQAExec PUBLIC EventQA)

install(TARGETS EventQA DESTINATION /usr/lib)
install(TARGETS EventQAExec DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/EventQA.h DESTINATION include)
#-L/opt/root/lib -lGui -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -Wl,-rpath,/opt/root/lib -pthread -lm -ldl -rdynamic
