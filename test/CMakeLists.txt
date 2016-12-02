cmake_minimum_required(VERSION 2.8)

find_package(CxxTest)

include_directories(${CXXTEST_INCLUDES} ${PROJECT_SOURCE_DIR}/include)

cxxtest_add_test(vector3dTest vector3dTest.cpp ${CMAKE_CURRENT_LIST_DIR}/vector3dTest.hpp)
cxxtest_add_test(vector2cTest vector2cTest.cpp ${CMAKE_CURRENT_LIST_DIR}/vector2cTest.hpp)

if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  # Using gnu
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
  # Using visual studio
endif()
