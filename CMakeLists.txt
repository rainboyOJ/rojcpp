# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目名称
project(rojcpp)

# 指定编译flag
set(CMAKE_CXX_FLAGS "-std=c++20")

# 设置宏
add_definitions(-D DEBUG)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D__NETCORE__DEBUG__)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

#aux_source_directory(. DIR_SRCS)
set(SERVER_SRCS main.cpp third-party/netcore/lib/easyloggingpp/easylogging++.cc)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
set(USER_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/__user_config.hpp)


set(NOT_ADD_NETCORE_TESTS 1)
#add_subdirectory(./third-party/netcore/)

# 得到mysql的lib的配置
execute_process(COMMAND mysql_config --cflags
    OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
    OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(./third-party/)
include_directories(./third-party/netcore/include/netcore)
include_directories(./third-party/netcore/lib)
include_directories(./third-party/cppjson/src)
include_directories(./third-party/sqlpoolcpp/sqlconn)
include_directories(./third-party/judgercpp/)
include_directories(./third-party/modern_cppdb/include)
include_directories(./lib)
include_directories(./jsonEntity)
include_directories(./routes)



if(EXISTS ${USER_CONFIG_PATH})
    message(STATUS "使用用户的配置文件 ${USER_CONFIG_PATH}")
    add_definitions(-D USER_CONFIG)
    add_compile_options(-include ${USER_CONFIG_PATH})
else()
    message(STATUS "使用默认配置文件 ${PROJECT_SOURCE_DIR}/__default_config.hpp")
endif(EXISTS ${USER_CONFIG_PATH})


add_executable(server ${SERVER_SRCS})




# 添加库
target_link_libraries(server pthread ${MYSQL_LIBS} hiredis)
target_compile_options(server PUBLIC ${MYSQL_CFLAGS})
