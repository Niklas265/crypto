# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/daniel/Downloads/clion-2019.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/daniel/Downloads/clion-2019.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniel/K/Crypto/AK/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniel/K/Crypto/AK/src/cmake-build-debug

# Include any dependencies generated for this target.
include Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/depend.make

# Include the progress variables for this target.
include Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/progress.make

# Include the compile flags for this target's objects.
include Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/flags.make

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/hellostudent-main.cpp.o: Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/flags.make
Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/hellostudent-main.cpp.o: ../Praktikum-CMake/Apps/hellostudent-main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/hellostudent-main.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hellostudent.dir/hellostudent-main.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps/hellostudent-main.cpp

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/hellostudent-main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hellostudent.dir/hellostudent-main.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps/hellostudent-main.cpp > CMakeFiles/hellostudent.dir/hellostudent-main.cpp.i

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/hellostudent-main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hellostudent.dir/hellostudent-main.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps/hellostudent-main.cpp -o CMakeFiles/hellostudent.dir/hellostudent-main.cpp.s

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/Greetings.cpp.o: Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/flags.make
Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/Greetings.cpp.o: ../Praktikum-CMake/Apps/Greetings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/Greetings.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hellostudent.dir/Greetings.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps/Greetings.cpp

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/Greetings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hellostudent.dir/Greetings.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps/Greetings.cpp > CMakeFiles/hellostudent.dir/Greetings.cpp.i

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/Greetings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hellostudent.dir/Greetings.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps/Greetings.cpp -o CMakeFiles/hellostudent.dir/Greetings.cpp.s

# Object files for target hellostudent
hellostudent_OBJECTS = \
"CMakeFiles/hellostudent.dir/hellostudent-main.cpp.o" \
"CMakeFiles/hellostudent.dir/Greetings.cpp.o"

# External object files for target hellostudent
hellostudent_EXTERNAL_OBJECTS =

bin/hellostudent: Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/hellostudent-main.cpp.o
bin/hellostudent: Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/Greetings.cpp.o
bin/hellostudent: Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/build.make
bin/hellostudent: Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../bin/hellostudent"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hellostudent.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/build: bin/hellostudent

.PHONY : Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/build

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/clean:
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps && $(CMAKE_COMMAND) -P CMakeFiles/hellostudent.dir/cmake_clean.cmake
.PHONY : Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/clean

Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/depend:
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/K/Crypto/AK/src /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Apps /home/daniel/K/Crypto/AK/src/cmake-build-debug /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Praktikum-CMake/Apps/CMakeFiles/hellostudent.dir/depend

