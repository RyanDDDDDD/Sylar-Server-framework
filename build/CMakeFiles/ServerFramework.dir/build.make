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
CMAKE_SOURCE_DIR = /home/ruidong/Desktop/ServerFramework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ruidong/Desktop/ServerFramework/build

# Include any dependencies generated for this target.
include CMakeFiles/ServerFramework.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ServerFramework.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ServerFramework.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ServerFramework.dir/flags.make

CMakeFiles/ServerFramework.dir/main.cpp.o: CMakeFiles/ServerFramework.dir/flags.make
CMakeFiles/ServerFramework.dir/main.cpp.o: ../main.cpp
CMakeFiles/ServerFramework.dir/main.cpp.o: CMakeFiles/ServerFramework.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruidong/Desktop/ServerFramework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ServerFramework.dir/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ServerFramework.dir/main.cpp.o -MF CMakeFiles/ServerFramework.dir/main.cpp.o.d -o CMakeFiles/ServerFramework.dir/main.cpp.o -c /home/ruidong/Desktop/ServerFramework/main.cpp

CMakeFiles/ServerFramework.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerFramework.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruidong/Desktop/ServerFramework/main.cpp > CMakeFiles/ServerFramework.dir/main.cpp.i

CMakeFiles/ServerFramework.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerFramework.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruidong/Desktop/ServerFramework/main.cpp -o CMakeFiles/ServerFramework.dir/main.cpp.s

# Object files for target ServerFramework
ServerFramework_OBJECTS = \
"CMakeFiles/ServerFramework.dir/main.cpp.o"

# External object files for target ServerFramework
ServerFramework_EXTERNAL_OBJECTS =

ServerFramework: CMakeFiles/ServerFramework.dir/main.cpp.o
ServerFramework: CMakeFiles/ServerFramework.dir/build.make
ServerFramework: CMakeFiles/ServerFramework.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruidong/Desktop/ServerFramework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ServerFramework"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ServerFramework.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ServerFramework.dir/build: ServerFramework
.PHONY : CMakeFiles/ServerFramework.dir/build

CMakeFiles/ServerFramework.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ServerFramework.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ServerFramework.dir/clean

CMakeFiles/ServerFramework.dir/depend:
	cd /home/ruidong/Desktop/ServerFramework/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruidong/Desktop/ServerFramework /home/ruidong/Desktop/ServerFramework /home/ruidong/Desktop/ServerFramework/build /home/ruidong/Desktop/ServerFramework/build /home/ruidong/Desktop/ServerFramework/build/CMakeFiles/ServerFramework.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ServerFramework.dir/depend
