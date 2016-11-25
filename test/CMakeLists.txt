cmake_minimum_required(VERSION 2.8)

include(CxxTest)

include_directories(${CXXTEST_INCLUDES})

cxx_test(VectorTest ${CMAKE_CURRENT_LIST_DIR}/VectorTest.hpp)
