project(spgInit)

cmake_minimum_required(VERSION 2.6)

set(spgInit_SRCS
    src/crystal.cpp
    src/elemInfo.cpp
    src/main.cpp
    src/spgInitCombinatorics.cpp
    src/spgInitOptions.cpp
    src/spgInit.cpp)

include_directories(${spgInit_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable (spgInit ${spgInit_SRCS})
