cmake_minimum_required(VERSION 3.10)  # 必须
set(CMAKE_CXX_STANDARD 17)	# C++14

project(Statistics)  #工程名


aux_source_directory(src SRCS) # 源文件

set(CMAKE_C_FLAGS "-O2") # 设置C工程的 CFLAGS
set(CMAKE_CXX_FLAGS "-O0 -Werror") # 设置C++ 工程的 CXX_FLAGS

add_executable(${PROJECT_NAME} ${SRCS}) # 生成可执行文件，这里程序名即为功能名

