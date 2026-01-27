cmake_minimum_required(VERSION 3.18)

project(MyMathTest)

find_package(MyMath CONFIG REQUIRED)

add_executable(main test.cpp)

target_link_libraries(main PRIVATE MyMath::MyMath)

# set(a "hello")
# if(a)
#     message("aaa")
# else()
#     message("noaaa")
# endif()

# if(${a})
#     message("aaa")
# else()
#     message("noaaa")
# endif()

# 使用变量中的列表
# set(FRUITS "apple;banana;cherry")  # CMake 列表是分号分隔的字符串
# foreach(fruit ${FRUITS})
#     message("Fruit: ${fruit}")
# endforeach()

# set(L1 "1")
# set(L2 "2;3;4")
# set(L3 "abc")
# foreach(X IN LISTS L1 L2 L3)    
#     message(STATUS ${X})
# endforeach() 

# set(FRUITS  "123" "apple;banana;cherry")  # CMake 列表是分号分隔的字符串
# foreach(fruit ${FRUITS})
#     message("Fruit: ${fruit}")
# endforeach()

# set(i 1)
# while(i LESS_EQUAL 5)
#     message(STATUS "i = ${i}")
#     math(EXPR i "${i} + 1") # i++    
# endwhile()

# foreach(fruit "apple;banana;cherry")
#     message("Fruit: ${fruit}")
# endforeach()

cmake_minimum_required(VERSION 3.18)

project(ListDemo)

set(SRCS "main.cpp;utils.cpp;math.cpp")

# 添加元素
list(APPEND SRCS "network.cpp")
message(STATUS "add:" ${SRCS})

# 移除元素
list(REMOVE_ITEM SRCS "math.cpp")
message(STATUS "remove:" ${SRCS})

# 排序
list(SORT SRCS)
message(STATUS "sort:" ${SRCS})

# 遍历列表
foreach(file ${SRCS})
    message(STATUS "file: ${file}")
endforeach(file ${SRCS})

# 求长度
list(LENGTH SRCS len)
message(STATUS "len: ${len}")