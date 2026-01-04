cmake_minimum_required(VERSION 3.18)

project(test
    VERSION 1.2.3
    LANGUAGES CXX
)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

add_executable(test test.cc)

# 安装可执行文件
include(GNUInstallDirs)
install(TARGETS test)

# name
message(STATUS "project_name: " ${PROJECT_NAME})
message(STATUS "camke_project_name: " ${CMAKE_PROJECT_NAME})
message(STATUS "project_lanuages: " ${PROJECT_LANUAGES})

# version
message(STATUS "project_version: " ${PROJECT_VERSION})
message(STATUS "project_version_major:" ${PROJECT_VERSION_MAJOR})
message(STATUS "project_version_minor: " ${PROJECT_VERSION_MINOR})
message(STATUS "project_version_patch: " ${PROJECT_VERSION_PATCH})

# dir
# 顶层的CMakeLists.txt所在的目录
message(STATUS "cmake_source_dir: " ${CMAKE_SOURCE_DIR})
# 顶层的CMakeLists.txt的构建目录
message(STATUS "cmake_binary_dir: " ${CMAKE_BINARY_DIR})
# 最近 project() 命令对应的 CMakeLists.txt 目录
message(STATUS "project_source_dir: " ${PROJECT_SOURCE_DIR})
# 最近 project() 命令对应的 CMakeLists.txt 构建目录
message(STATUS "project_binary_dir: " ${PROJECT_BINARY_DIR})
# 当前正在处理的 CMakeLists.txt 所在的目录（根据add_subdirectory()会变化）
message(STATUS "cmake_current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "cmake_current_binary_dir: " ${CMAKE_CURRENT_BINARY_DIR})

# file
message(STATUS "cmake_current_list_dir: " ${CMAKE_CURRENT_LIST_DIR})
message(STATUS "cmake_current_list_file: " ${CMAKE_CURRENT_LIST_FILE})

message(STATUS "cmake_install_bindir: " ${CMAKE_INSTALL_BINDIR})