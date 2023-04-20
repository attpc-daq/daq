# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /storage/ATTPC/daq-dev-/src/PacketDecoder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/daq-dev-/src/PacketDecoder/build

# Include any dependencies generated for this target.
include CMakeFiles/PacketDecoder.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PacketDecoder.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PacketDecoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PacketDecoder.dir/flags.make

CMakeFiles/PacketDecoder.dir/PacketDecoder.o: CMakeFiles/PacketDecoder.dir/flags.make
CMakeFiles/PacketDecoder.dir/PacketDecoder.o: ../PacketDecoder.cpp
CMakeFiles/PacketDecoder.dir/PacketDecoder.o: CMakeFiles/PacketDecoder.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-/src/PacketDecoder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PacketDecoder.dir/PacketDecoder.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PacketDecoder.dir/PacketDecoder.o -MF CMakeFiles/PacketDecoder.dir/PacketDecoder.o.d -o CMakeFiles/PacketDecoder.dir/PacketDecoder.o -c /storage/ATTPC/daq-dev-/src/PacketDecoder/PacketDecoder.cpp

CMakeFiles/PacketDecoder.dir/PacketDecoder.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PacketDecoder.dir/PacketDecoder.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-/src/PacketDecoder/PacketDecoder.cpp > CMakeFiles/PacketDecoder.dir/PacketDecoder.i

CMakeFiles/PacketDecoder.dir/PacketDecoder.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PacketDecoder.dir/PacketDecoder.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-/src/PacketDecoder/PacketDecoder.cpp -o CMakeFiles/PacketDecoder.dir/PacketDecoder.s

# Object files for target PacketDecoder
PacketDecoder_OBJECTS = \
"CMakeFiles/PacketDecoder.dir/PacketDecoder.o"

# External object files for target PacketDecoder
PacketDecoder_EXTERNAL_OBJECTS = \
"/storage/ATTPC/daq-dev-/src/PacketDecoder/build/CMakeFiles/PacketDecoder_dict.dir/PacketDecoder_dict.o"

libPacketDecoder.so: CMakeFiles/PacketDecoder.dir/PacketDecoder.o
libPacketDecoder.so: CMakeFiles/PacketDecoder_dict.dir/PacketDecoder_dict.o
libPacketDecoder.so: CMakeFiles/PacketDecoder.dir/build.make
libPacketDecoder.so: /opt/root/lib/libNet.so.6.26.04
libPacketDecoder.so: /opt/root/lib/libRIO.so.6.26.04
libPacketDecoder.so: /opt/root/lib/libThread.so.6.26.04
libPacketDecoder.so: /opt/root/lib/libCore.so.6.26.04
libPacketDecoder.so: CMakeFiles/PacketDecoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/ATTPC/daq-dev-/src/PacketDecoder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libPacketDecoder.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PacketDecoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PacketDecoder.dir/build: libPacketDecoder.so
.PHONY : CMakeFiles/PacketDecoder.dir/build

CMakeFiles/PacketDecoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PacketDecoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PacketDecoder.dir/clean

CMakeFiles/PacketDecoder.dir/depend:
	cd /storage/ATTPC/daq-dev-/src/PacketDecoder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/daq-dev-/src/PacketDecoder /storage/ATTPC/daq-dev-/src/PacketDecoder /storage/ATTPC/daq-dev-/src/PacketDecoder/build /storage/ATTPC/daq-dev-/src/PacketDecoder/build /storage/ATTPC/daq-dev-/src/PacketDecoder/build/CMakeFiles/PacketDecoder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PacketDecoder.dir/depend

