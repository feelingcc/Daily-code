# 1 搜索文件列表
file(GLOB SRC_LISTS "./*.cpp")

# 2 添加构建目标
add_executable(main ${SRC_LISTS})

# 3 添加依赖库列表
target_link_libraries(main PRIVATE MyMath)
# target_include_directories(main
#     PRIVATE ${CMAKE_SOURCE_DIR}/my_lib/include
# )
# message(STATUS "${CMAKE_SOURCE_DIR}/my_lib/include")

# 4 设置输出路径
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
