cmake_minimum_required(VERSION 2.8)

project(dicek)

set(serial "1.0.0")
set(soserial "1")

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(dicek SHARED dummy.cpp)

if(NOT without-test)
enable_testing()
add_subdirectory(test)
endif()
