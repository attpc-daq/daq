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
CMAKE_SOURCE_DIR = /storage/ATTPC/daq-dev-/src/SiTCP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage/ATTPC/daq-dev-/src/SiTCP/build

# Include any dependencies generated for this target.
include CMakeFiles/SiTCP.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SiTCP.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SiTCP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SiTCP.dir/flags.make

CMakeFiles/SiTCP.dir/SiTCP.o: CMakeFiles/SiTCP.dir/flags.make
CMakeFiles/SiTCP.dir/SiTCP.o: ../SiTCP.cpp
CMakeFiles/SiTCP.dir/SiTCP.o: CMakeFiles/SiTCP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage/ATTPC/daq-dev-/src/SiTCP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SiTCP.dir/SiTCP.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SiTCP.dir/SiTCP.o -MF CMakeFiles/SiTCP.dir/SiTCP.o.d -o CMakeFiles/SiTCP.dir/SiTCP.o -c /storage/ATTPC/daq-dev-/src/SiTCP/SiTCP.cpp

CMakeFiles/SiTCP.dir/SiTCP.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SiTCP.dir/SiTCP.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage/ATTPC/daq-dev-/src/SiTCP/SiTCP.cpp > CMakeFiles/SiTCP.dir/SiTCP.i

CMakeFiles/SiTCP.dir/SiTCP.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SiTCP.dir/SiTCP.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage/ATTPC/daq-dev-/src/SiTCP/SiTCP.cpp -o CMakeFiles/SiTCP.dir/SiTCP.s

# Object files for target SiTCP
SiTCP_OBJECTS = \
"CMakeFiles/SiTCP.dir/SiTCP.o"

# External object files for target SiTCP
SiTCP_EXTERNAL_OBJECTS = \
"/storage/ATTPC/daq-dev-/src/SiTCP/build/CMakeFiles/SiTCP_dict.dir/SiTCP_dict.o"

libSiTCP.so: CMakeFiles/SiTCP.dir/SiTCP.o
libSiTCP.so: CMakeFiles/SiTCP_dict.dir/SiTCP_dict.o
libSiTCP.so: CMakeFiles/SiTCP.dir/build.make
libSiTCP.so: /opt/root/lib/libNet.so.6.26.04
libSiTCP.so: /opt/root/lib/libRIO.so.6.26.04
libSiTCP.so: /opt/root/lib/libThread.so.6.26.04
libSiTCP.so: /opt/root/lib/libCore.so.6.26.04
libSiTCP.so: CMakeFiles/SiTCP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage/ATTPC/daq-dev-/src/SiTCP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libSiTCP.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SiTCP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SiTCP.dir/build: libSiTCP.so
.PHONY : CMakeFiles/SiTCP.dir/build

CMakeFiles/SiTCP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SiTCP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SiTCP.dir/clean

CMakeFiles/SiTCP.dir/depend:
	cd /storage/ATTPC/daq-dev-/src/SiTCP/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage/ATTPC/daq-dev-/src/SiTCP /storage/ATTPC/daq-dev-/src/SiTCP /storage/ATTPC/daq-dev-/src/SiTCP/build /storage/ATTPC/daq-dev-/src/SiTCP/build /storage/ATTPC/daq-dev-/src/SiTCP/build/CMakeFiles/SiTCP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SiTCP.dir/depend

