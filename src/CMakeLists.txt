#include directories helps to include header files
include_directories(".")
include_directories(${INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

#get the source filenames to compile
file(GLOB_RECURSE prem_SOURCES *.cpp)

find_library(MGL_LIBRARY mgl)
find_library(MGLGLUT_LIBRARY mgl-glut)
add_executable(${PRJ_NAME} ${prem_SOURCES})


#target_link_libraries(${PRJ_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(${PRJ_NAME} GL GLU glut)
target_link_libraries(${PRJ_NAME} ${MGL_LIBRARY} ${MGLGLUT_LIBRARY})
