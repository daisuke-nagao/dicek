cmake_minimum_required(VERSION 3.16)

project(dicek VERSION 0.3.0 LANGUAGES CXX)

add_library(dicek INTERFACE)
target_include_directories(dicek
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
target_compile_features(dicek INTERFACE cxx_std_17)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR dicek_BUILD_TESTING) AND BUILD_TESTING)
  include(GoogleTest)
  add_subdirectory(test)
endif()
