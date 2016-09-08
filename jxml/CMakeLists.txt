cmake_minimum_required(VERSION 3.3)
project(jxml)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES jxml.h jxml.c mem_pool.c mem_pool.h)
add_executable(jxml ${SOURCE_FILES})