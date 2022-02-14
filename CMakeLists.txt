cmake_minimum_required(VERSION 3.16)

project(dicek VERSION 0.3.0 LANGUAGES CXX)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

configure_file(
  "${PROJECT_SOURCE_DIR}/version.hpp.in"
  "${PROJECT_BINARY_DIR}/include/dicek/version.hpp"
)

add_library(dicek INTERFACE)
target_include_directories(dicek
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
target_compile_features(dicek INTERFACE cxx_std_17)

add_library(${PROJECT_NAME}::dicek ALIAS dicek)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR dicek_BUILD_TESTING) AND BUILD_TESTING)
  option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
  include(GoogleTest)
  add_subdirectory(test)
endif()
