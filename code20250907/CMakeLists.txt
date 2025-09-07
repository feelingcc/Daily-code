cmake_minimum_required(VERSION 3.1)
project("test")
set(target_file ${CMAKE_CURRENT_SOURCE_DIR}/test.cc)
add_executable(test ${target_file})
