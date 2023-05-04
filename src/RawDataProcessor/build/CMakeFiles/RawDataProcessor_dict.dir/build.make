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
CMAKE_SOURCE_DIR = /storage/ATTPC/DAQ/src/RawDataProcessor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/DAQ/src/RawDataProcessor/build

# Include any dependencies generated for this target.
include CMakeFiles/RawDataProcessor_dict.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RawDataProcessor_dict.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RawDataProcessor_dict.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RawDataProcessor_dict.dir/flags.make

RawDataProcessor_dict.cxx: ../Linkdef.h
RawDataProcessor_dict.cxx: ../RawDataProcessor.h
RawDataProcessor_dict.cxx: ../RawDataProcessor.h
RawDataProcessor_dict.cxx: ../Linkdef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/storage/ATTPC/DAQ/src/RawDataProcessor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating RawDataProcessor_dict.cxx, libRawDataProcessor_rdict.pcm, libRawDataProcessor.rootmap"
	/usr/bin/cmake -E env LD_LIBRARY_PATH=/opt/root/lib:/usr/local/lib: /opt/root/bin/rootcling -v2 -f RawDataProcessor_dict.cxx -s /storage/ATTPC/DAQ/src/RawDataProcessor/build/libRawDataProcessor.so -rml libRawDataProcessor.so -rmf /storage/ATTPC/DAQ/src/RawDataProcessor/build/libRawDataProcessor.rootmap -compilerI/usr/include/c++/11 -compilerI/usr/include/x86_64-linux-gnu/c++/11 -compilerI/usr/include/c++/11/backward -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -I/storage/ATTPC/DAQ/src/RawDataProcessor -I/storage/ATTPC/DAQ/src/RawDataProcessor -I/opt/root/include RawDataProcessor.h /storage/ATTPC/DAQ/src/RawDataProcessor/Linkdef.h

libRawDataProcessor_rdict.pcm: RawDataProcessor_dict.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate libRawDataProcessor_rdict.pcm

libRawDataProcessor.rootmap: RawDataProcessor_dict.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate libRawDataProcessor.rootmap

CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o: CMakeFiles/RawDataProcessor_dict.dir/flags.make
CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o: RawDataProcessor_dict.cxx
CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o: CMakeFiles/RawDataProcessor_dict.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/DAQ/src/RawDataProcessor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o -MF CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o.d -o CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o -c /storage/ATTPC/DAQ/src/RawDataProcessor/build/RawDataProcessor_dict.cxx

CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/DAQ/src/RawDataProcessor/build/RawDataProcessor_dict.cxx > CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.i

CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/DAQ/src/RawDataProcessor/build/RawDataProcessor_dict.cxx -o CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.s

RawDataProcessor_dict: CMakeFiles/RawDataProcessor_dict.dir/RawDataProcessor_dict.o
RawDataProcessor_dict: CMakeFiles/RawDataProcessor_dict.dir/build.make
.PHONY : RawDataProcessor_dict

# Rule to build all files generated by this target.
CMakeFiles/RawDataProcessor_dict.dir/build: RawDataProcessor_dict
.PHONY : CMakeFiles/RawDataProcessor_dict.dir/build

CMakeFiles/RawDataProcessor_dict.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RawDataProcessor_dict.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RawDataProcessor_dict.dir/clean

CMakeFiles/RawDataProcessor_dict.dir/depend: RawDataProcessor_dict.cxx
CMakeFiles/RawDataProcessor_dict.dir/depend: libRawDataProcessor.rootmap
CMakeFiles/RawDataProcessor_dict.dir/depend: libRawDataProcessor_rdict.pcm
	cd /storage/ATTPC/DAQ/src/RawDataProcessor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/DAQ/src/RawDataProcessor /storage/ATTPC/DAQ/src/RawDataProcessor /storage/ATTPC/DAQ/src/RawDataProcessor/build /storage/ATTPC/DAQ/src/RawDataProcessor/build /storage/ATTPC/DAQ/src/RawDataProcessor/build/CMakeFiles/RawDataProcessor_dict.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RawDataProcessor_dict.dir/depend

