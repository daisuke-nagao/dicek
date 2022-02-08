cmake_minimum_required(VERSION 3.16)

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")
mark_as_advanced(
  BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
  gmock_build_tests gtest_build_samples gtest_build_tests
  gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

add_executable(scalar_traitsTest scalar_traitsTest.cpp)
set_target_properties(scalar_traitsTest PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
target_link_libraries(scalar_traitsTest dicek gtest_main)
include(GoogleTest)
gtest_discover_tests(scalar_traitsTest)

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
