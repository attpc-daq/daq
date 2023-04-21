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
CMAKE_SOURCE_DIR = /storage/ATTPC/daq-dev-/src/RawEventProcessor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/daq-dev-/src/RawEventProcessor/build

# Include any dependencies generated for this target.
include CMakeFiles/RawEventProcessor_dict.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RawEventProcessor_dict.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RawEventProcessor_dict.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RawEventProcessor_dict.dir/flags.make

RawEventProcessor_dict.cxx: ../Linkdef.h
RawEventProcessor_dict.cxx: ../RawEventProcessor.h
RawEventProcessor_dict.cxx: ../RawEventProcessor.h
RawEventProcessor_dict.cxx: ../Linkdef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/storage/ATTPC/daq-dev-/src/RawEventProcessor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating RawEventProcessor_dict.cxx, libRawEventProcessor_rdict.pcm, libRawEventProcessor.rootmap"
	/usr/bin/cmake -E env LD_LIBRARY_PATH=/opt/root/lib:/usr/local/lib: /opt/root/bin/rootcling -v2 -f RawEventProcessor_dict.cxx -s /storage/ATTPC/daq-dev-/src/RawEventProcessor/build/libRawEventProcessor.so -rml libRawEventProcessor.so -rmf /storage/ATTPC/daq-dev-/src/RawEventProcessor/build/libRawEventProcessor.rootmap -compilerI/usr/include/c++/11 -compilerI/usr/include/x86_64-linux-gnu/c++/11 -compilerI/usr/include/c++/11/backward -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -I/storage/ATTPC/daq-dev-/src/RawEventProcessor -I/storage/ATTPC/daq-dev-/src/RawEventProcessor -I/opt/root/include RawEventProcessor.h /storage/ATTPC/daq-dev-/src/RawEventProcessor/Linkdef.h

libRawEventProcessor_rdict.pcm: RawEventProcessor_dict.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate libRawEventProcessor_rdict.pcm

libRawEventProcessor.rootmap: RawEventProcessor_dict.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate libRawEventProcessor.rootmap

CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o: CMakeFiles/RawEventProcessor_dict.dir/flags.make
CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o: RawEventProcessor_dict.cxx
CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o: CMakeFiles/RawEventProcessor_dict.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-/src/RawEventProcessor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o -MF CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o.d -o CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o -c /storage/ATTPC/daq-dev-/src/RawEventProcessor/build/RawEventProcessor_dict.cxx

CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-/src/RawEventProcessor/build/RawEventProcessor_dict.cxx > CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.i

CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-/src/RawEventProcessor/build/RawEventProcessor_dict.cxx -o CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.s

RawEventProcessor_dict: CMakeFiles/RawEventProcessor_dict.dir/RawEventProcessor_dict.o
RawEventProcessor_dict: CMakeFiles/RawEventProcessor_dict.dir/build.make
.PHONY : RawEventProcessor_dict

# Rule to build all files generated by this target.
CMakeFiles/RawEventProcessor_dict.dir/build: RawEventProcessor_dict
.PHONY : CMakeFiles/RawEventProcessor_dict.dir/build

CMakeFiles/RawEventProcessor_dict.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RawEventProcessor_dict.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RawEventProcessor_dict.dir/clean

CMakeFiles/RawEventProcessor_dict.dir/depend: RawEventProcessor_dict.cxx
CMakeFiles/RawEventProcessor_dict.dir/depend: libRawEventProcessor.rootmap
CMakeFiles/RawEventProcessor_dict.dir/depend: libRawEventProcessor_rdict.pcm
	cd /storage/ATTPC/daq-dev-/src/RawEventProcessor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/daq-dev-/src/RawEventProcessor /storage/ATTPC/daq-dev-/src/RawEventProcessor /storage/ATTPC/daq-dev-/src/RawEventProcessor/build /storage/ATTPC/daq-dev-/src/RawEventProcessor/build /storage/ATTPC/daq-dev-/src/RawEventProcessor/build/CMakeFiles/RawEventProcessor_dict.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RawEventProcessor_dict.dir/depend
