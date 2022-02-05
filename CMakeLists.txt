cmake_minimum_required(VERSION 3.16)

project(dicek VERSION 0.1.0 LANGUAGES CXX)

if(BUILD_TESTING)
enable_testing()
add_subdirectory(googletest)
add_subdirectory(test)
endif()
