# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/ohadkoren/Clion/clion-2018.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ohadkoren/Clion/clion-2018.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ohadkoren/SPLABS/LAB6/Task1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/taskFile.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/taskFile.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/taskFile.dir/flags.make

CMakeFiles/taskFile.dir/myshell.c.o: CMakeFiles/taskFile.dir/flags.make
CMakeFiles/taskFile.dir/myshell.c.o: ../myshell.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/taskFile.dir/myshell.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/taskFile.dir/myshell.c.o   -c /home/ohadkoren/SPLABS/LAB6/Task1/myshell.c

CMakeFiles/taskFile.dir/myshell.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/taskFile.dir/myshell.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ohadkoren/SPLABS/LAB6/Task1/myshell.c > CMakeFiles/taskFile.dir/myshell.c.i

CMakeFiles/taskFile.dir/myshell.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/taskFile.dir/myshell.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ohadkoren/SPLABS/LAB6/Task1/myshell.c -o CMakeFiles/taskFile.dir/myshell.c.s

CMakeFiles/taskFile.dir/LineParser.c.o: CMakeFiles/taskFile.dir/flags.make
CMakeFiles/taskFile.dir/LineParser.c.o: ../LineParser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/taskFile.dir/LineParser.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/taskFile.dir/LineParser.c.o   -c /home/ohadkoren/SPLABS/LAB6/Task1/LineParser.c

CMakeFiles/taskFile.dir/LineParser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/taskFile.dir/LineParser.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ohadkoren/SPLABS/LAB6/Task1/LineParser.c > CMakeFiles/taskFile.dir/LineParser.c.i

CMakeFiles/taskFile.dir/LineParser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/taskFile.dir/LineParser.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ohadkoren/SPLABS/LAB6/Task1/LineParser.c -o CMakeFiles/taskFile.dir/LineParser.c.s

# Object files for target taskFile
taskFile_OBJECTS = \
"CMakeFiles/taskFile.dir/myshell.c.o" \
"CMakeFiles/taskFile.dir/LineParser.c.o"

# External object files for target taskFile
taskFile_EXTERNAL_OBJECTS =

taskFile: CMakeFiles/taskFile.dir/myshell.c.o
taskFile: CMakeFiles/taskFile.dir/LineParser.c.o
taskFile: CMakeFiles/taskFile.dir/build.make
taskFile: CMakeFiles/taskFile.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable taskFile"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/taskFile.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/taskFile.dir/build: taskFile

.PHONY : CMakeFiles/taskFile.dir/build

CMakeFiles/taskFile.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/taskFile.dir/cmake_clean.cmake
.PHONY : CMakeFiles/taskFile.dir/clean

CMakeFiles/taskFile.dir/depend:
	cd /home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ohadkoren/SPLABS/LAB6/Task1 /home/ohadkoren/SPLABS/LAB6/Task1 /home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug /home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug /home/ohadkoren/SPLABS/LAB6/Task1/cmake-build-debug/CMakeFiles/taskFile.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/taskFile.dir/depend
