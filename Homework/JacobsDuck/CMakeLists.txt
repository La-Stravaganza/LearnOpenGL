cmake_minimum_required(VERSION 3.5)
project(JacobsDuck)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp include/global.h src/global.cpp include/callbacks.h src/callbacks.cpp )
add_executable(JacobsDuck ${SOURCE_FILES})

# set self-defined modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

include_directories(include ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(JacobsDuck ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLM_LIBRARIES})

