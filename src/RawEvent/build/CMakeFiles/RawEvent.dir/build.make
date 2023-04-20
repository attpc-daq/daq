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
CMAKE_SOURCE_DIR = /storage/ATTPC/daq-dev-fastQueue/src/RawEvent

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build

# Include any dependencies generated for this target.
include CMakeFiles/RawEvent.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RawEvent.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RawEvent.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RawEvent.dir/flags.make

CMakeFiles/RawEvent.dir/RawEvent.o: CMakeFiles/RawEvent.dir/flags.make
CMakeFiles/RawEvent.dir/RawEvent.o: ../RawEvent.cpp
CMakeFiles/RawEvent.dir/RawEvent.o: CMakeFiles/RawEvent.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RawEvent.dir/RawEvent.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RawEvent.dir/RawEvent.o -MF CMakeFiles/RawEvent.dir/RawEvent.o.d -o CMakeFiles/RawEvent.dir/RawEvent.o -c /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/RawEvent.cpp

CMakeFiles/RawEvent.dir/RawEvent.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RawEvent.dir/RawEvent.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/RawEvent.cpp > CMakeFiles/RawEvent.dir/RawEvent.i

CMakeFiles/RawEvent.dir/RawEvent.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RawEvent.dir/RawEvent.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/RawEvent.cpp -o CMakeFiles/RawEvent.dir/RawEvent.s

CMakeFiles/RawEvent.dir/Channel.o: CMakeFiles/RawEvent.dir/flags.make
CMakeFiles/RawEvent.dir/Channel.o: ../Channel.cpp
CMakeFiles/RawEvent.dir/Channel.o: CMakeFiles/RawEvent.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RawEvent.dir/Channel.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RawEvent.dir/Channel.o -MF CMakeFiles/RawEvent.dir/Channel.o.d -o CMakeFiles/RawEvent.dir/Channel.o -c /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/Channel.cpp

CMakeFiles/RawEvent.dir/Channel.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RawEvent.dir/Channel.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/Channel.cpp > CMakeFiles/RawEvent.dir/Channel.i

CMakeFiles/RawEvent.dir/Channel.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RawEvent.dir/Channel.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/Channel.cpp -o CMakeFiles/RawEvent.dir/Channel.s

# Object files for target RawEvent
RawEvent_OBJECTS = \
"CMakeFiles/RawEvent.dir/RawEvent.o" \
"CMakeFiles/RawEvent.dir/Channel.o"

# External object files for target RawEvent
RawEvent_EXTERNAL_OBJECTS = \
"/storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build/CMakeFiles/RawEvent_dict.dir/RawEvent_dict.o"

libRawEvent.so: CMakeFiles/RawEvent.dir/RawEvent.o
libRawEvent.so: CMakeFiles/RawEvent.dir/Channel.o
libRawEvent.so: CMakeFiles/RawEvent_dict.dir/RawEvent_dict.o
libRawEvent.so: CMakeFiles/RawEvent.dir/build.make
libRawEvent.so: /opt/root/lib/libNet.so.6.26.04
libRawEvent.so: /opt/root/lib/libRIO.so.6.26.04
libRawEvent.so: /opt/root/lib/libThread.so.6.26.04
libRawEvent.so: /opt/root/lib/libCore.so.6.26.04
libRawEvent.so: CMakeFiles/RawEvent.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libRawEvent.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RawEvent.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RawEvent.dir/build: libRawEvent.so
.PHONY : CMakeFiles/RawEvent.dir/build

CMakeFiles/RawEvent.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RawEvent.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RawEvent.dir/clean

CMakeFiles/RawEvent.dir/depend:
	cd /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/daq-dev-fastQueue/src/RawEvent /storage/ATTPC/daq-dev-fastQueue/src/RawEvent /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build /storage/ATTPC/daq-dev-fastQueue/src/RawEvent/build/CMakeFiles/RawEvent.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RawEvent.dir/depend

