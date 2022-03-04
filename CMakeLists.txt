# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目名称
project(rojcpp)

# 指定编译flag
set(CMAKE_CXX_FLAGS "-std=c++17")

# 设置宏
add_definitions(-D DEBUG)

#aux_source_directory(. DIR_SRCS)
set(SERVER_SRCS main.cpp)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
set(USER_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/__user_config.hpp)


set(NOT_ADD_NETCORE_TESTS 1)
add_subdirectory(./third-party/netcore/)

# 得到mysql的lib的配置
execute_process(COMMAND mysql_config --cflags
    OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
    OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(./third-party/netcore/core)
include_directories(./third-party/netcore/lib)
add_executable(server ${SERVER_SRCS})


# 添加库
target_link_libraries(server netcore pthread ${MYSQL_LIBS})
target_compile_options(server PUBLIC ${MYSQL_CFLAGS})
