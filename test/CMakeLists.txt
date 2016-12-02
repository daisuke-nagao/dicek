cmake_minimum_required(VERSION 2.8)

include(CxxTest)

include_directories(${CXXTEST_INCLUDES} ${PROJECT_SOURCE_DIR}/include)

cxx_test(vector3dTest ${CMAKE_CURRENT_LIST_DIR}/vector3dTest.hpp)
cxx_test(vector2cTest ${CMAKE_CURRENT_LIST_DIR}/vector2cTest.hpp)

if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  # Using gnu
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
  # Using visual studio
endif()
