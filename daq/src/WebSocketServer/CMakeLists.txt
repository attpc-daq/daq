message(STATUS "Enter WebSocketServer folder")
# 设置项目名称和版本
project(WebSocketServer)

find_package(Boost REQUIRED COMPONENTS system)
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/opt/root/include")

add_library(WebSocketServer SHARED WebSocketServer.cpp)

target_link_libraries(WebSocketServer PRIVATE ${Boost_LIBRARIES})

install(TARGETS WebSocketServer DESTINATION /usr/lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/WebSocketServer.h DESTINATION include)
