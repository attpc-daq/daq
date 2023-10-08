message(STATUS "Enter DAQHandler folder")
# 设置项目名称和版本
project(DAQHandler)

find_package(Boost REQUIRED COMPONENTS system)
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/opt/root/include")

add_library(DAQHandler SHARED DAQHandler.cpp)

target_link_libraries(DAQHandler PRIVATE ${Boost_LIBRARIES} SiTCP DataProcessor EventQA)

install(TARGETS DAQHandler DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/DAQHandler.h DESTINATION include)
