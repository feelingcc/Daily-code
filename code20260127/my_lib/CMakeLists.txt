# 1 收集源代码
file(GLOB SRC_LISTS "./src/*.cpp")

# 2 添加构建目标
add_library(MyMath STATIC ${SRC_LISTS})

# 3 设置库的使用者必须包含的头文件搜索路径 
target_include_directories(MyMath INTERFACE
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>" # /usr/local/include
)

# 4 设置库的默认输出路径
set_target_properties(MyMath PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 5 安装静态库
include(GNUInstallDirs)
install(TARGETS MyMath
    EXPORT MyMathTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR} # /usr/local/lib
)

# 6 安装头文件
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/math # /usr/local/include/math/math.h
    FILES_MATCHING PATTERN "*.h" # 指定只安装 .h 文件
)

# 7 安装导出目标集合到构建目录
export(EXPORT MyMathTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/MyMathTargets.cmake
)

# 8 安装导出目标集合到安装目录
install(EXPORT MyMathTargets
    FILE MyMathTargets.cmake
    NAMESPACE MyMath::  # MyMath::MyMath
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyMath # /usr/local/lib/cmake/MyMath/MyMathTargets.cmake
)

# 9 生成 find_package 需要的配置文件
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MyMathConfig.cmake
    INSTALL_DESTINATION "lib/cmake/MyMath"
)

# 10 安装配置文件到 cmake 标准的安装路径
install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/MyMathConfig.cmake
    DESTINATION "lib/cmake/MyMath"
)
