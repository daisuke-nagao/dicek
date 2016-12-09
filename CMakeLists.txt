cmake_minimum_required(VERSION 2.8)

project(dicek CXX)

if(NOT without-test)
enable_testing()
add_subdirectory(test)
endif()
