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
CMAKE_SOURCE_DIR = /storage/ATTPC/daq-dev-/src/EventMaker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/daq-dev-/src/EventMaker/build

# Include any dependencies generated for this target.
include CMakeFiles/EventMaker.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/EventMaker.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/EventMaker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EventMaker.dir/flags.make

CMakeFiles/EventMaker.dir/EventMaker.o: CMakeFiles/EventMaker.dir/flags.make
CMakeFiles/EventMaker.dir/EventMaker.o: ../EventMaker.cpp
CMakeFiles/EventMaker.dir/EventMaker.o: CMakeFiles/EventMaker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-/src/EventMaker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/EventMaker.dir/EventMaker.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/EventMaker.dir/EventMaker.o -MF CMakeFiles/EventMaker.dir/EventMaker.o.d -o CMakeFiles/EventMaker.dir/EventMaker.o -c /storage/ATTPC/daq-dev-/src/EventMaker/EventMaker.cpp

CMakeFiles/EventMaker.dir/EventMaker.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EventMaker.dir/EventMaker.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-/src/EventMaker/EventMaker.cpp > CMakeFiles/EventMaker.dir/EventMaker.i

CMakeFiles/EventMaker.dir/EventMaker.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EventMaker.dir/EventMaker.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-/src/EventMaker/EventMaker.cpp -o CMakeFiles/EventMaker.dir/EventMaker.s

# Object files for target EventMaker
EventMaker_OBJECTS = \
"CMakeFiles/EventMaker.dir/EventMaker.o"

# External object files for target EventMaker
EventMaker_EXTERNAL_OBJECTS = \
"/storage/ATTPC/daq-dev-/src/EventMaker/build/CMakeFiles/EventMaker_dict.dir/EventMaker_dict.o"

libEventMaker.so: CMakeFiles/EventMaker.dir/EventMaker.o
libEventMaker.so: CMakeFiles/EventMaker_dict.dir/EventMaker_dict.o
libEventMaker.so: CMakeFiles/EventMaker.dir/build.make
libEventMaker.so: /opt/root/lib/libNet.so.6.26.04
libEventMaker.so: /opt/root/lib/libRIO.so.6.26.04
libEventMaker.so: /opt/root/lib/libThread.so.6.26.04
libEventMaker.so: /opt/root/lib/libCore.so.6.26.04
libEventMaker.so: CMakeFiles/EventMaker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/ATTPC/daq-dev-/src/EventMaker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libEventMaker.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EventMaker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EventMaker.dir/build: libEventMaker.so
.PHONY : CMakeFiles/EventMaker.dir/build

CMakeFiles/EventMaker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EventMaker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EventMaker.dir/clean

CMakeFiles/EventMaker.dir/depend:
	cd /storage/ATTPC/daq-dev-/src/EventMaker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/daq-dev-/src/EventMaker /storage/ATTPC/daq-dev-/src/EventMaker /storage/ATTPC/daq-dev-/src/EventMaker/build /storage/ATTPC/daq-dev-/src/EventMaker/build /storage/ATTPC/daq-dev-/src/EventMaker/build/CMakeFiles/EventMaker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EventMaker.dir/depend
