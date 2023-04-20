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
CMAKE_SOURCE_DIR = /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build

# Include any dependencies generated for this target.
include CMakeFiles/MessageBuffer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MessageBuffer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MessageBuffer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MessageBuffer.dir/flags.make

CMakeFiles/MessageBuffer.dir/MessageBuffer.o: CMakeFiles/MessageBuffer.dir/flags.make
CMakeFiles/MessageBuffer.dir/MessageBuffer.o: ../MessageBuffer.cpp
CMakeFiles/MessageBuffer.dir/MessageBuffer.o: CMakeFiles/MessageBuffer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MessageBuffer.dir/MessageBuffer.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MessageBuffer.dir/MessageBuffer.o -MF CMakeFiles/MessageBuffer.dir/MessageBuffer.o.d -o CMakeFiles/MessageBuffer.dir/MessageBuffer.o -c /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/MessageBuffer.cpp

CMakeFiles/MessageBuffer.dir/MessageBuffer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MessageBuffer.dir/MessageBuffer.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/MessageBuffer.cpp > CMakeFiles/MessageBuffer.dir/MessageBuffer.i

CMakeFiles/MessageBuffer.dir/MessageBuffer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MessageBuffer.dir/MessageBuffer.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/MessageBuffer.cpp -o CMakeFiles/MessageBuffer.dir/MessageBuffer.s

# Object files for target MessageBuffer
MessageBuffer_OBJECTS = \
"CMakeFiles/MessageBuffer.dir/MessageBuffer.o"

# External object files for target MessageBuffer
MessageBuffer_EXTERNAL_OBJECTS = \
"/storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build/CMakeFiles/MessageBuffer_dict.dir/MessageBuffer_dict.o"

libMessageBuffer.so: CMakeFiles/MessageBuffer.dir/MessageBuffer.o
libMessageBuffer.so: CMakeFiles/MessageBuffer_dict.dir/MessageBuffer_dict.o
libMessageBuffer.so: CMakeFiles/MessageBuffer.dir/build.make
libMessageBuffer.so: /opt/root/lib/libNet.so.6.26.04
libMessageBuffer.so: /opt/root/lib/libRIO.so.6.26.04
libMessageBuffer.so: /opt/root/lib/libThread.so.6.26.04
libMessageBuffer.so: /opt/root/lib/libCore.so.6.26.04
libMessageBuffer.so: CMakeFiles/MessageBuffer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libMessageBuffer.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MessageBuffer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MessageBuffer.dir/build: libMessageBuffer.so
.PHONY : CMakeFiles/MessageBuffer.dir/build

CMakeFiles/MessageBuffer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MessageBuffer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MessageBuffer.dir/clean

CMakeFiles/MessageBuffer.dir/depend:
	cd /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build /storage/ATTPC/daq-dev-fastQueue/src/TMessageBuffer/build/CMakeFiles/MessageBuffer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MessageBuffer.dir/depend

