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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nguyenquan263/Documents/Development/cs455-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cs455-lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cs455-lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cs455-lib.dir/flags.make

CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.o: CMakeFiles/cs455-lib.dir/flags.make
CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.o: ../src/dao/userDao/userDao.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.o -c /Users/nguyenquan263/Documents/Development/cs455-project/src/dao/userDao/userDao.cpp

CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nguyenquan263/Documents/Development/cs455-project/src/dao/userDao/userDao.cpp > CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.i

CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nguyenquan263/Documents/Development/cs455-project/src/dao/userDao/userDao.cpp -o CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.s

# Object files for target cs455-lib
cs455__lib_OBJECTS = \
"CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.o"

# External object files for target cs455-lib
cs455__lib_EXTERNAL_OBJECTS =

libcs455-lib.a: CMakeFiles/cs455-lib.dir/src/dao/userDao/userDao.cpp.o
libcs455-lib.a: CMakeFiles/cs455-lib.dir/build.make
libcs455-lib.a: CMakeFiles/cs455-lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libcs455-lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/cs455-lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cs455-lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cs455-lib.dir/build: libcs455-lib.a

.PHONY : CMakeFiles/cs455-lib.dir/build

CMakeFiles/cs455-lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cs455-lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cs455-lib.dir/clean

CMakeFiles/cs455-lib.dir/depend:
	cd /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nguyenquan263/Documents/Development/cs455-project /Users/nguyenquan263/Documents/Development/cs455-project /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug/CMakeFiles/cs455-lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cs455-lib.dir/depend

