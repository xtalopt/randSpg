project(spgGen)

cmake_minimum_required(VERSION 2.6)

set(spgGen_SRCS
    src/crystal.cpp
    src/elemInfo.cpp
    src/main.cpp
    src/spgGenCombinatorics.cpp
    src/spgGenOptions.cpp
    src/spgGen.cpp)

include_directories(${spgGen_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable (spgGen ${spgGen_SRCS})
