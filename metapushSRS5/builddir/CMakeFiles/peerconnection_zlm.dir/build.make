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
include CMakeFiles/peerconnection_zlm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/peerconnection_zlm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/peerconnection_zlm.dir/flags.make

CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.o: CMakeFiles/peerconnection_zlm.dir/flags.make
CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.o: ../peerconnection_zlm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.o -c /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/peerconnection_zlm.cpp

CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/peerconnection_zlm.cpp > CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.i

CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/peerconnection_zlm.cpp -o CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.s

# Object files for target peerconnection_zlm
peerconnection_zlm_OBJECTS = \
"CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.o"

# External object files for target peerconnection_zlm
peerconnection_zlm_EXTERNAL_OBJECTS =

peerconnection_zlm: CMakeFiles/peerconnection_zlm.dir/peerconnection_zlm.cpp.o
peerconnection_zlm: CMakeFiles/peerconnection_zlm.dir/build.make
peerconnection_zlm: ../libmetartc5.a
peerconnection_zlm: ../libmetartccore5.a
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libavcodec.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libavformat.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libavutil.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libswscale.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libvorbis.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libvorbisenc.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libtheora.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libgsm.so
peerconnection_zlm: /usr/lib/x86_64-linux-gnu/libz.so
peerconnection_zlm: CMakeFiles/peerconnection_zlm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable peerconnection_zlm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/peerconnection_zlm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/peerconnection_zlm.dir/build: peerconnection_zlm

.PHONY : CMakeFiles/peerconnection_zlm.dir/build

CMakeFiles/peerconnection_zlm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/peerconnection_zlm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/peerconnection_zlm.dir/clean

CMakeFiles/peerconnection_zlm.dir/depend:
	cd /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5 /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5 /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir /home/yhx/Documents/metaRTC5.0-Ubuntu-push-stream-demo/metapushSRS5/builddir/CMakeFiles/peerconnection_zlm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/peerconnection_zlm.dir/depend

