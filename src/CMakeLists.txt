cmake_minimum_required(VERSION 2.8)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(dicek MODULE)

install(FILES Vector.hpp DESTINATION include)

