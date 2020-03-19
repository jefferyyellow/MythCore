cmake_minimum_required (VERSION 2.8)
PROJECT(TcpServer)
#定义宏
ADD_DEFINITIONS(-DMYTH_OS_UNIX)
#包含的头文件目录
INCLUDE_DIRECTORIES(../MythCore/include)
#设置C++的编译标志
SET(CMAKE_CXX_FLAGS "-g -pipe -Werror -DMEDIUM_SIZE -DMYTH_OS_UNIX -D__DEBUG__ -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'")
#指定库目录
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Debug)
SET(EXECUTABLE_OUTPUT_PATH ../../Debug)
#连接MyCore和rt库
LINK_LIBRARIES(MythCore)
SET(SRC_LIST main.cpp)
#message(STATUS "src list*****************************" ${SRC_LIST})
#生成MythCore
ADD_EXECUTABLE(CrashSave ${SRC_LIST})
