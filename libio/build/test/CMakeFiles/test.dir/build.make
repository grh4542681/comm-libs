# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake

# The command to remove a file.
RM = /home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ezgaoro/workspace/comm-libs/libio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ezgaoro/workspace/comm-libs/libio/build

# Utility rule file for test.

# Include the progress variables for this target.
include test/CMakeFiles/test.dir/progress.make

test: test/CMakeFiles/test.dir/build.make

.PHONY : test

# Rule to build all files generated by this target.
test/CMakeFiles/test.dir/build: test

.PHONY : test/CMakeFiles/test.dir/build

test/CMakeFiles/test.dir/clean:
	cd /home/ezgaoro/workspace/comm-libs/libio/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test.dir/clean

test/CMakeFiles/test.dir/depend:
	cd /home/ezgaoro/workspace/comm-libs/libio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ezgaoro/workspace/comm-libs/libio /home/ezgaoro/workspace/comm-libs/libio/test /home/ezgaoro/workspace/comm-libs/libio/build /home/ezgaoro/workspace/comm-libs/libio/build/test /home/ezgaoro/workspace/comm-libs/libio/build/test/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test.dir/depend

