project(randSpg)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(randSpg_SRCS
    src/crystal.cpp
    src/elemInfo.cpp
    src/randSpgCombinatorics.cpp
    src/randSpgOptions.cpp
    src/randSpg.cpp)

include_directories(${randSpg_SOURCE_DIR}/include)

add_library(RandSpgLib ${randSpg_SRCS})

# C++11 is required. MSVC should not need a flag
if(UNIX OR MINGW)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif(UNIX OR MINGW)

option(BUILD_RELEASE
       "Whether to compile with optimizations or not"
       ON)

# Let's perform some optimizations!
if (BUILD_RELEASE)
  message("-- Release build. Please use -DBUILD_RELEASE=OFF if you wish to use a debug build")
  if(UNIX OR MINGW)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif(UNIX OR MINGW)
  if(MSVC)
    SET(CMAKE_BUILD_TYPE "Release")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
  endif(MSVC)
else (BUILD_RELEASE)
  message("-- Debug build")

endif (BUILD_RELEASE)

add_executable (randSpg src/main.cpp)
target_link_libraries (randSpg RandSpgLib)

option( BUILD_CGI
        "Whether to compile the CGI handler in addition to the randSpg code."
        OFF )
if(BUILD_CGI)
  add_executable (randSpg.cgi cgi/randSpg-cgi.cpp)
  target_link_libraries (randSpg.cgi RandSpgLib)
endif(BUILD_CGI)

option(BUILD_PYTHON_BINDINGS
       "Whether or not to build the Python bindings using Pybind11."
       OFF)
if(BUILD_PYTHON_BINDINGS)
  add_subdirectory(python)
endif(BUILD_PYTHON_BINDINGS)
