cmake_minimum_required(VERSION 3.3)
project(LightingMaps)

# switch to clang
set(CMAKE_C_COMPILER               "/usr/bin/clang")
set(CMAKE_C_FLAGS                  "-Wall -std=c99")
set(CMAKE_C_FLAGS_DEBUG            "-g")
set(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")

set(CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_AR                       "/usr/bin/llvm-ar")
set(CMAKE_LINKER                   "/usr/bin/llvm-ld")
set(CMAKE_NM                       "/usr/bin/llvm-nm")
set(CMAKE_OBJDUMP                  "/usr/bin/llvm-objdump")
set(CMAKE_RANLIB                   "/usr/bin/llvm-ranlib")

# add executables
set(SOURCE_FILES main.cpp Shader.h Shader.cpp)
add_executable(LightingMaps ${SOURCE_FILES})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(LightingMaps ${OPENGL_LIBRARIES})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(LightingMaps ${GLEW_LIBRARIES})

# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(LightingMaps ${GLFW_LIBRARIES})

# set self-defined modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# SOIL
find_package(SOIL REQUIRED)
include_directories(${SOIL_INCLUDE_DIRS})
target_link_libraries(LightingMaps ${SOIL_LIBRARIES})

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(LightingMaps ${GLM_LIBRARIES})