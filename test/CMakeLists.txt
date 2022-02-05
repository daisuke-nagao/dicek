cmake_minimum_required(VERSION 3.16)


#find_package(CxxTest)
#
#include_directories(${CXXTEST_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
#
#file(GLOB TEST_SOURCES "*Test.hpp" "*Test.h" "*test.h")
#foreach( FILEPATH ${TEST_SOURCES} )
#  get_filename_component( FILENAME_WE ${FILEPATH} NAME_WE )
#  cxxtest_add_test(${FILENAME_WE} "${FILENAME_WE}.cpp" ${FILEPATH})
#endforeach()
#
#if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
#  # Using gnu
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#elseif(${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
#  # Using visual studio
#endif()
