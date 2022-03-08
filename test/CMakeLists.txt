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

macro(package_add_test TESTNAME)
  # create an exectuable in which the tests will be stored
  add_executable(${TESTNAME} ${ARGN})
  # link the Google test infrastructure, mocking library, and a default main fuction to
  # the test executable.  Remove g_test_main if writing your own main function.
  target_link_libraries(${TESTNAME} dicek gtest gmock gtest_main)
  # gtest_discover_tests replaces gtest_add_tests,
  # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
  gtest_discover_tests(${TESTNAME}
    # set a working directory so your project root so that you can find test data via paths relative to the project root
    WORKING_DIRECTORY ${PROJECT_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
  )
  set_target_properties(${TESTNAME} PROPERTIES
    FOLDER tests
    CXX_EXTENSIONS OFF
  )
endmacro()

package_add_test(scalar_traitsTest scalar_traitsTest.cpp)
package_add_test(vectorTest vectorTest.cpp)
package_add_test(vectorIteratorTest vectorIteratorTest.cpp)

if("cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
package_add_test(scalar_traitsTest_cxx20 scalar_traitsTest.cpp)
target_compile_features(scalar_traitsTest_cxx20 PRIVATE cxx_std_20)
package_add_test(vectorTest_cxx20 vectorTest.cpp)
target_compile_features(vectorTest_cxx20 PRIVATE cxx_std_20)
package_add_test(vectorIteratorTest_cxx20 vectorTest.cpp)
target_compile_features(vectorIteratorTest_cxx20 PRIVATE cxx_std_20)
endif()
