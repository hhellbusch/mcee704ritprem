#include directories helps to include header files
include_directories(".")
include_directories(${INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

#get the source filenames to compile
file(GLOB_RECURSE prem_SOURCES *.cpp)

add_executable(${PRJ_NAME} ${prem_SOURCES})


#target_link_libraries(${PRJ_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

target_link_libraries(${PRJ_NAME} ${CMAKE_SOURCE_DIR}/lib/matplotpp.a)
target_link_libraries(${PRJ_NAME} GL GLU glut)