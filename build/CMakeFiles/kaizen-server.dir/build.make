# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/simone/kaizen-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/simone/kaizen-server/build

# Include any dependencies generated for this target.
include CMakeFiles/kaizen-server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/kaizen-server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/kaizen-server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kaizen-server.dir/flags.make

CMakeFiles/kaizen-server.dir/main.cpp.o: CMakeFiles/kaizen-server.dir/flags.make
CMakeFiles/kaizen-server.dir/main.cpp.o: /home/simone/kaizen-server/main.cpp
CMakeFiles/kaizen-server.dir/main.cpp.o: CMakeFiles/kaizen-server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/simone/kaizen-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kaizen-server.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/kaizen-server.dir/main.cpp.o -MF CMakeFiles/kaizen-server.dir/main.cpp.o.d -o CMakeFiles/kaizen-server.dir/main.cpp.o -c /home/simone/kaizen-server/main.cpp

CMakeFiles/kaizen-server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/kaizen-server.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/simone/kaizen-server/main.cpp > CMakeFiles/kaizen-server.dir/main.cpp.i

CMakeFiles/kaizen-server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/kaizen-server.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/simone/kaizen-server/main.cpp -o CMakeFiles/kaizen-server.dir/main.cpp.s

# Object files for target kaizen-server
kaizen__server_OBJECTS = \
"CMakeFiles/kaizen-server.dir/main.cpp.o"

# External object files for target kaizen-server
kaizen__server_EXTERNAL_OBJECTS =

kaizen-server: CMakeFiles/kaizen-server.dir/main.cpp.o
kaizen-server: CMakeFiles/kaizen-server.dir/build.make
kaizen-server: CMakeFiles/kaizen-server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/simone/kaizen-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kaizen-server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kaizen-server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kaizen-server.dir/build: kaizen-server
.PHONY : CMakeFiles/kaizen-server.dir/build

CMakeFiles/kaizen-server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kaizen-server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kaizen-server.dir/clean

CMakeFiles/kaizen-server.dir/depend:
	cd /home/simone/kaizen-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/simone/kaizen-server /home/simone/kaizen-server /home/simone/kaizen-server/build /home/simone/kaizen-server/build /home/simone/kaizen-server/build/CMakeFiles/kaizen-server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/kaizen-server.dir/depend
