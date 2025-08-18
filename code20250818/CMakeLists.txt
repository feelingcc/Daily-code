cmake_minimum_required(VERSION 3.1)
project("test")
# 设置默认构建类型为 Release（如果未通过命令行指定）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(target_file ${CMAKE_CURRENT_SOURCE_DIR}/test.cc)
add_executable(test ${target_file})
