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
CMAKE_SOURCE_DIR = /home/ezgaoro/workspace/comm-libs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ezgaoro/workspace/comm-libs/build

# Utility rule file for libbase.

# Include the progress variables for this target.
include CMakeFiles/libbase.dir/progress.make

CMakeFiles/libbase: CMakeFiles/libbase-complete


CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-install
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-mkdir
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-download
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-update
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-patch
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-configure
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-build
CMakeFiles/libbase-complete: libbase-prefix/src/libbase-stamp/libbase-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'libbase'"
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/CMakeFiles
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/CMakeFiles/libbase-complete
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-done

libbase-prefix/src/libbase-stamp/libbase-install: libbase-prefix/src/libbase-stamp/libbase-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'libbase'"
	cd /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build && /usr/bin/gmake install
	cd /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build && /home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-install

libbase-prefix/src/libbase-stamp/libbase-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'libbase'"
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/libbase
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/libbase-prefix
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/tmp
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E make_directory /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-mkdir

libbase-prefix/src/libbase-stamp/libbase-download: libbase-prefix/src/libbase-stamp/libbase-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'libbase'"
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E echo_append
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-download

libbase-prefix/src/libbase-stamp/libbase-update: libbase-prefix/src/libbase-stamp/libbase-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'libbase'"
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E echo_append
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-update

libbase-prefix/src/libbase-stamp/libbase-patch: libbase-prefix/src/libbase-stamp/libbase-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'libbase'"
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E echo_append
	/home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-patch

libbase-prefix/src/libbase-stamp/libbase-configure: libbase-prefix/tmp/libbase-cfgcmd.txt
libbase-prefix/src/libbase-stamp/libbase-configure: libbase-prefix/src/libbase-stamp/libbase-update
libbase-prefix/src/libbase-stamp/libbase-configure: libbase-prefix/src/libbase-stamp/libbase-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'libbase'"
	cd /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build && /home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -DBUILD_TEST=ON -DBUILD_STATIC_LIBS=OFF -DOUT=/home/ezgaoro/workspace/comm-libs/output /home/ezgaoro/workspace/comm-libs/libbase
	cd /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build && /home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-configure

libbase-prefix/src/libbase-stamp/libbase-build: libbase-prefix/src/libbase-stamp/libbase-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ezgaoro/workspace/comm-libs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'libbase'"
	cd /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build && $(MAKE)
	cd /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-build && /home/ezgaoro/workspace/tools/cmake-3.17.3/bin/cmake -E touch /home/ezgaoro/workspace/comm-libs/build/libbase-prefix/src/libbase-stamp/libbase-build

libbase: CMakeFiles/libbase
libbase: CMakeFiles/libbase-complete
libbase: libbase-prefix/src/libbase-stamp/libbase-install
libbase: libbase-prefix/src/libbase-stamp/libbase-mkdir
libbase: libbase-prefix/src/libbase-stamp/libbase-download
libbase: libbase-prefix/src/libbase-stamp/libbase-update
libbase: libbase-prefix/src/libbase-stamp/libbase-patch
libbase: libbase-prefix/src/libbase-stamp/libbase-configure
libbase: libbase-prefix/src/libbase-stamp/libbase-build
libbase: CMakeFiles/libbase.dir/build.make

.PHONY : libbase

# Rule to build all files generated by this target.
CMakeFiles/libbase.dir/build: libbase

.PHONY : CMakeFiles/libbase.dir/build

CMakeFiles/libbase.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libbase.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libbase.dir/clean

CMakeFiles/libbase.dir/depend:
	cd /home/ezgaoro/workspace/comm-libs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ezgaoro/workspace/comm-libs /home/ezgaoro/workspace/comm-libs /home/ezgaoro/workspace/comm-libs/build /home/ezgaoro/workspace/comm-libs/build /home/ezgaoro/workspace/comm-libs/build/CMakeFiles/libbase.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libbase.dir/depend

