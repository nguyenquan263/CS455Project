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
include CMakeFiles/cs455.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cs455.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cs455.dir/flags.make

CMakeFiles/cs455.dir/src/App.cpp.o: CMakeFiles/cs455.dir/flags.make
CMakeFiles/cs455.dir/src/App.cpp.o: ../src/App.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cs455.dir/src/App.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cs455.dir/src/App.cpp.o -c /Users/nguyenquan263/Documents/Development/cs455-project/src/App.cpp

CMakeFiles/cs455.dir/src/App.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cs455.dir/src/App.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nguyenquan263/Documents/Development/cs455-project/src/App.cpp > CMakeFiles/cs455.dir/src/App.cpp.i

CMakeFiles/cs455.dir/src/App.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cs455.dir/src/App.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nguyenquan263/Documents/Development/cs455-project/src/App.cpp -o CMakeFiles/cs455.dir/src/App.cpp.s

# Object files for target cs455
cs455_OBJECTS = \
"CMakeFiles/cs455.dir/src/App.cpp.o"

# External object files for target cs455
cs455_EXTERNAL_OBJECTS =

cs455: CMakeFiles/cs455.dir/src/App.cpp.o
cs455: CMakeFiles/cs455.dir/build.make
cs455: libcs455-lib.a
cs455: /usr/local/lib/oatpp-1.2.5/liboatpp-swagger.a
cs455: /usr/local/lib/oatpp-1.2.5/liboatpp-mongo.a
cs455: /usr/local/lib/oatpp-1.2.5/liboatpp-test.a
cs455: /usr/local/lib/oatpp-1.2.5/liboatpp.a
cs455: /usr/local/lib/libmongocxx.3.6.2.dylib
cs455: /usr/local/lib/libbsoncxx.3.6.2.dylib
cs455: CMakeFiles/cs455.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cs455"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cs455.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cs455.dir/build: cs455

.PHONY : CMakeFiles/cs455.dir/build

CMakeFiles/cs455.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cs455.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cs455.dir/clean

CMakeFiles/cs455.dir/depend:
	cd /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nguyenquan263/Documents/Development/cs455-project /Users/nguyenquan263/Documents/Development/cs455-project /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug /Users/nguyenquan263/Documents/Development/cs455-project/cmake-build-debug/CMakeFiles/cs455.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cs455.dir/depend
