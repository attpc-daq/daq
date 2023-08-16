# 设置项目名称和版本
project(myproject)

# 指定最低 CMake 版本
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find_package(ROOT REQUIRED COMPONENTS RIO Net Hist)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})



# 添加可执行文件
add_executable(myprogram main.cpp)

# target_link_libraries(myprogram PUBLIC )

