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
CMAKE_SOURCE_DIR = /storage/ATTPC/DAQ_dev/src/DataProcessor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/DAQ_dev/src/DataProcessor/build

# Include any dependencies generated for this target.
include CMakeFiles/DataProcessor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DataProcessor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DataProcessor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DataProcessor.dir/flags.make

CMakeFiles/DataProcessor.dir/DataProcessor.o: CMakeFiles/DataProcessor.dir/flags.make
CMakeFiles/DataProcessor.dir/DataProcessor.o: ../DataProcessor.cpp
CMakeFiles/DataProcessor.dir/DataProcessor.o: CMakeFiles/DataProcessor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/DAQ_dev/src/DataProcessor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DataProcessor.dir/DataProcessor.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DataProcessor.dir/DataProcessor.o -MF CMakeFiles/DataProcessor.dir/DataProcessor.o.d -o CMakeFiles/DataProcessor.dir/DataProcessor.o -c /storage/ATTPC/DAQ_dev/src/DataProcessor/DataProcessor.cpp

CMakeFiles/DataProcessor.dir/DataProcessor.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DataProcessor.dir/DataProcessor.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/DAQ_dev/src/DataProcessor/DataProcessor.cpp > CMakeFiles/DataProcessor.dir/DataProcessor.i

CMakeFiles/DataProcessor.dir/DataProcessor.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DataProcessor.dir/DataProcessor.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/DAQ_dev/src/DataProcessor/DataProcessor.cpp -o CMakeFiles/DataProcessor.dir/DataProcessor.s

# Object files for target DataProcessor
DataProcessor_OBJECTS = \
"CMakeFiles/DataProcessor.dir/DataProcessor.o"

# External object files for target DataProcessor
DataProcessor_EXTERNAL_OBJECTS = \
"/storage/ATTPC/DAQ_dev/src/DataProcessor/build/CMakeFiles/DataProcessor_dict.dir/DataProcessor_dict.o"

libDataProcessor.so: CMakeFiles/DataProcessor.dir/DataProcessor.o
libDataProcessor.so: CMakeFiles/DataProcessor_dict.dir/DataProcessor_dict.o
libDataProcessor.so: CMakeFiles/DataProcessor.dir/build.make
libDataProcessor.so: /opt/root/lib/libRHTTP.so.6.26.04
libDataProcessor.so: /opt/root/lib/libGraf.so.6.26.04
libDataProcessor.so: /opt/root/lib/libHist.so.6.26.04
libDataProcessor.so: /opt/root/lib/libMatrix.so.6.26.04
libDataProcessor.so: /opt/root/lib/libMathCore.so.6.26.04
libDataProcessor.so: /opt/root/lib/libImt.so.6.26.04
libDataProcessor.so: /opt/root/lib/libMultiProc.so.6.26.04
libDataProcessor.so: /opt/root/lib/libNet.so.6.26.04
libDataProcessor.so: /opt/root/lib/libRIO.so.6.26.04
libDataProcessor.so: /opt/root/lib/libThread.so.6.26.04
libDataProcessor.so: /opt/root/lib/libCore.so.6.26.04
libDataProcessor.so: CMakeFiles/DataProcessor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/ATTPC/DAQ_dev/src/DataProcessor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libDataProcessor.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DataProcessor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DataProcessor.dir/build: libDataProcessor.so
.PHONY : CMakeFiles/DataProcessor.dir/build

CMakeFiles/DataProcessor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DataProcessor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DataProcessor.dir/clean

CMakeFiles/DataProcessor.dir/depend:
	cd /storage/ATTPC/DAQ_dev/src/DataProcessor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/DAQ_dev/src/DataProcessor /storage/ATTPC/DAQ_dev/src/DataProcessor /storage/ATTPC/DAQ_dev/src/DataProcessor/build /storage/ATTPC/DAQ_dev/src/DataProcessor/build /storage/ATTPC/DAQ_dev/src/DataProcessor/build/CMakeFiles/DataProcessor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DataProcessor.dir/depend

