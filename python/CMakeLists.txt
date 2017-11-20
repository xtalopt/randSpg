cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(PythonInterp 3 REQUIRED)

set(PYBIND11_CPP_STANDARD -std=c++11)

find_package(pybind11 REQUIRED)

message(STATUS "Found pybind11: ${pybind11_VERSION}")
include_directories(${PYBIND11_INCLUDE_DIR})

pybind11_add_module(pyrandspg MODULE pyrandspg.cpp)
target_link_libraries(pyrandspg PRIVATE RandSpgLib)
