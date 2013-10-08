#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.8.3)

set(PRJ_NAME ritprem)
#indicate type of the project as executable, otherwise it is a library
set(PRJ_TYPE_EXECUTABLE 1)
#allways build libarary (with object files)
set(PRJ_BUILD_LIBRARY 1)
#main file name - used to filter this with object files
set(PRJ_MAIN_FILE "ritprem.cpp")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(${PRJ_NAME})

add_subdirectory(src)

enable_testing()

#
# Prevent building in the source directory by default
#
if( ALLOW_SAME_DIRECTORY )
elseif( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
	option( ALLOW_SAME_DIRECTORY "Allow CMake to build in the source directory." OFF )
	message( FATAL_ERROR
		"Do not use the source directory to build your files, instead delete CMakeCache.txt, create a separate folder and build there.\n"
		"Example: (build in subdir 'build' and install to source dir)\n"
		"  rm -f CMakeCache.txt\n"
		"  mkdir build\n"
		"  cd build\n"
		"  cmake -G\"Unix Makefiles\" -DINSTALL_TO_SOURCE=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo ..\n"
		"  make install\n"
		"  cd ..\n"
		"  rm -rf build\n"
		"To skip this check, set ALLOW_SAME_DIRECTORY to ON (-DALLOW_SAME_DIRECTORY=ON)" )
endif()

################

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

link_directories(lib)

