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
CMAKE_SOURCE_DIR = /home/dev/final-project/final-project-Sriram-Y

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dev/final-project/final-project-Sriram-Y/build

# Include any dependencies generated for this target.
include CMakeFiles/audioeqdriver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/audioeqdriver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/audioeqdriver.dir/flags.make

CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.o: CMakeFiles/audioeqdriver.dir/flags.make
CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.o: ../src/audioeqdriver.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/final-project/final-project-Sriram-Y/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.o   -c /home/dev/final-project/final-project-Sriram-Y/src/audioeqdriver.c

CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dev/final-project/final-project-Sriram-Y/src/audioeqdriver.c > CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.i

CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dev/final-project/final-project-Sriram-Y/src/audioeqdriver.c -o CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.s

CMakeFiles/audioeqdriver.dir/src/audio_processing.c.o: CMakeFiles/audioeqdriver.dir/flags.make
CMakeFiles/audioeqdriver.dir/src/audio_processing.c.o: ../src/audio_processing.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/final-project/final-project-Sriram-Y/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/audioeqdriver.dir/src/audio_processing.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/audioeqdriver.dir/src/audio_processing.c.o   -c /home/dev/final-project/final-project-Sriram-Y/src/audio_processing.c

CMakeFiles/audioeqdriver.dir/src/audio_processing.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/audioeqdriver.dir/src/audio_processing.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dev/final-project/final-project-Sriram-Y/src/audio_processing.c > CMakeFiles/audioeqdriver.dir/src/audio_processing.c.i

CMakeFiles/audioeqdriver.dir/src/audio_processing.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/audioeqdriver.dir/src/audio_processing.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dev/final-project/final-project-Sriram-Y/src/audio_processing.c -o CMakeFiles/audioeqdriver.dir/src/audio_processing.c.s

# Object files for target audioeqdriver
audioeqdriver_OBJECTS = \
"CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.o" \
"CMakeFiles/audioeqdriver.dir/src/audio_processing.c.o"

# External object files for target audioeqdriver
audioeqdriver_EXTERNAL_OBJECTS =

../audioeqdriver: CMakeFiles/audioeqdriver.dir/src/audioeqdriver.c.o
../audioeqdriver: CMakeFiles/audioeqdriver.dir/src/audio_processing.c.o
../audioeqdriver: CMakeFiles/audioeqdriver.dir/build.make
../audioeqdriver: /usr/lib/x86_64-linux-gnu/libasound.so
../audioeqdriver: CMakeFiles/audioeqdriver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dev/final-project/final-project-Sriram-Y/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../audioeqdriver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/audioeqdriver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/audioeqdriver.dir/build: ../audioeqdriver

.PHONY : CMakeFiles/audioeqdriver.dir/build

CMakeFiles/audioeqdriver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/audioeqdriver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/audioeqdriver.dir/clean

CMakeFiles/audioeqdriver.dir/depend:
	cd /home/dev/final-project/final-project-Sriram-Y/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dev/final-project/final-project-Sriram-Y /home/dev/final-project/final-project-Sriram-Y /home/dev/final-project/final-project-Sriram-Y/build /home/dev/final-project/final-project-Sriram-Y/build /home/dev/final-project/final-project-Sriram-Y/build/CMakeFiles/audioeqdriver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/audioeqdriver.dir/depend

