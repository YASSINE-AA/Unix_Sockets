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
CMAKE_SOURCE_DIR = /home/yassine-ahmed-ali/Documents/GooeyGUI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yassine-ahmed-ali/Documents/GooeyGUI/build

# Utility rule file for check.

# Include any custom commands dependencies for this target.
include third_party/cjson/CMakeFiles/check.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/cjson/CMakeFiles/check.dir/progress.make

third_party/cjson/CMakeFiles/check: third_party/cjson/cJSON_test
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI/build/third_party/cjson && /usr/bin/ctest --output-on-failure

check: third_party/cjson/CMakeFiles/check
check: third_party/cjson/CMakeFiles/check.dir/build.make
.PHONY : check

# Rule to build all files generated by this target.
third_party/cjson/CMakeFiles/check.dir/build: check
.PHONY : third_party/cjson/CMakeFiles/check.dir/build

third_party/cjson/CMakeFiles/check.dir/clean:
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI/build/third_party/cjson && $(CMAKE_COMMAND) -P CMakeFiles/check.dir/cmake_clean.cmake
.PHONY : third_party/cjson/CMakeFiles/check.dir/clean

third_party/cjson/CMakeFiles/check.dir/depend:
	cd /home/yassine-ahmed-ali/Documents/GooeyGUI/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yassine-ahmed-ali/Documents/GooeyGUI /home/yassine-ahmed-ali/Documents/GooeyGUI/third_party/cjson /home/yassine-ahmed-ali/Documents/GooeyGUI/build /home/yassine-ahmed-ali/Documents/GooeyGUI/build/third_party/cjson /home/yassine-ahmed-ali/Documents/GooeyGUI/build/third_party/cjson/CMakeFiles/check.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : third_party/cjson/CMakeFiles/check.dir/depend

