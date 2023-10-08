cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project (event VERSION 1.0.0)

set(GUISocketDir src/GUISocket)

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

install(DIRECTORY ${GUISocketDir} DESTINATION "/usr/local/lib/python3.10/dist-packages/")

# add_compile_options(-fno-elide-constructors)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")   # 设置可执行文件的输出目录

add_subdirectory(src/SiTCP)
add_subdirectory(src/Event)
add_subdirectory(src/RawEvent)
add_subdirectory(src/EventConverter)
add_subdirectory(src/TMessageBuffer)
add_subdirectory(src/TMessageSocket)
add_subdirectory(src/PacketDecoder)
add_subdirectory(src/ParameterGenerator)
add_subdirectory(src/DataProcessor)
add_subdirectory(src/EventQA)
add_subdirectory(src/AutoSocket)
add_subdirectory(src/DAQHandler)
add_subdirectory(src/WebSocketServer)
add_subdirectory(src)
