cmake_minimum_required(VERSION 3.1)
project("test")
set(target_file ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)

# add_executable 会自动在当前源目录中查找文件，所以通常不需要写完整路径
add_executable(test ${target_file})