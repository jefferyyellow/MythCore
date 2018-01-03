cmake_minimum_required (VERSION 2.8)
PROJECT(MYTHCORE)
#定义宏
ADD_DEFINITIONS(-DMYTH_OS_UNIX)
#包含的头文件目录
INCLUDE_DIRECTORIES(include ../dep/mysql/include)
#设置C++的编译标志
SET(CMAKE_CXX_FLAGS "-g -pipe -Werror -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'")
#指定库目录
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../dep/mysql/lib)
#SET(EXECUTABLE_OUTPUT_PATH ../../)
#连接mysql库
LINK_LIBRARIES(mysqlclient)
#src目录下的CMakeLists加入
ADD_SUBDIRECTORY(src)
#message(STATUS "src list*****************************" ${SRC_LIST})
#生成MythCore
ADD_LIBRARY(MythCore STATIC ${SRC_LIST})
