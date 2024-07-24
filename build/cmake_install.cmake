# Install script for directory: /storage/ATTPC/DAQ

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/python3.10/dist-packages//GUISocket")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/lib/python3.10/dist-packages/" TYPE DIRECTORY FILES "/storage/ATTPC/DAQ/src/GUISocket")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/storage/ATTPC/DAQ/build/src/SiTCP/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/Event/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/RawEvent/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/EventConverter/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/TMessageBuffer/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/TMessageSocket/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/PacketDecoder/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/ParameterGenerator/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/DataProcessor/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/EventQA/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/AutoSocket/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/DAQHandler/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/WebSocketServer/cmake_install.cmake")
  include("/storage/ATTPC/DAQ/build/src/BufferTP/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/storage/ATTPC/DAQ/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
