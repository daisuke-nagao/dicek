cmake_minimum_required(VERSION 3.16)

project(dicek VERSION 0.1.0 LANGUAGES CXX)

add_library(dicek INTERFACE)
target_include_directories(dicek
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

if(BUILD_TESTING)
enable_testing()
add_subdirectory(test)
endif()
