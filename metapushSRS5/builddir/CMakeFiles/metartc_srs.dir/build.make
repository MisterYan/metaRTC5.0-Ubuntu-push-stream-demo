# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir

# Include any dependencies generated for this target.
include CMakeFiles/metartc_srs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/metartc_srs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/metartc_srs.dir/flags.make

CMakeFiles/metartc_srs.dir/metartc_srs.cpp.o: CMakeFiles/metartc_srs.dir/flags.make
CMakeFiles/metartc_srs.dir/metartc_srs.cpp.o: ../metartc_srs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/metartc_srs.dir/metartc_srs.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/metartc_srs.dir/metartc_srs.cpp.o -c /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/metartc_srs.cpp

CMakeFiles/metartc_srs.dir/metartc_srs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/metartc_srs.dir/metartc_srs.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/metartc_srs.cpp > CMakeFiles/metartc_srs.dir/metartc_srs.cpp.i

CMakeFiles/metartc_srs.dir/metartc_srs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/metartc_srs.dir/metartc_srs.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/metartc_srs.cpp -o CMakeFiles/metartc_srs.dir/metartc_srs.cpp.s

# Object files for target metartc_srs
metartc_srs_OBJECTS = \
"CMakeFiles/metartc_srs.dir/metartc_srs.cpp.o"

# External object files for target metartc_srs
metartc_srs_EXTERNAL_OBJECTS =

metartc_srs: CMakeFiles/metartc_srs.dir/metartc_srs.cpp.o
metartc_srs: CMakeFiles/metartc_srs.dir/build.make
metartc_srs: ../libmetartc5.a
metartc_srs: ../libmetartccore5.a
metartc_srs: CMakeFiles/metartc_srs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable metartc_srs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/metartc_srs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/metartc_srs.dir/build: metartc_srs

.PHONY : CMakeFiles/metartc_srs.dir/build

CMakeFiles/metartc_srs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/metartc_srs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/metartc_srs.dir/clean

CMakeFiles/metartc_srs.dir/depend:
	cd /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5 /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5 /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir/CMakeFiles/metartc_srs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/metartc_srs.dir/depend

