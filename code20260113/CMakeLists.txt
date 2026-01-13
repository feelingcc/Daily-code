# 1 设置最低的CMake版本要求
cmake_minimum_required(VERSION 3.18)

# 2.设置项目名称
project(Test)

# 3.添加构建目标
add_executable(main main.cpp)

# 4.集成单元测试
include(CTest)
add_test(
	NAME testAdd
	COMMAND main
)

# 5.本地安装
include(GNUInstallDirs)
install(TARGETS main)

# 6.打包
include(CPack)

