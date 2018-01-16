cmake_minimum_required (VERSION 2.8)
PROJECT(TcpServer)
#定义宏
ADD_DEFINITIONS(-DMYTH_OS_UNIX)
#包含的头文件目录
INCLUDE_DIRECTORIES(../MythCore/include ../dep/tinyxml)
#设置C++的编译标志
SET(CMAKE_CXX_FLAGS "-g -pipe -Werror -DMEDIUM_SIZE -DMYTH_OS_UNIX -D__DEBUG__ -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'")
#指定库目录
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Debug)
SET(EXECUTABLE_OUTPUT_PATH ../../Debug)
#连接MyCore和rt库
LINK_LIBRARIES(MythCore rt)
SET(SRC_LIST tcpserver.cpp main.cpp ../dep/tinyxml/tinyxml2.cpp)
#message(STATUS "src list*****************************" ${SRC_LIST})
#生成MythCore
ADD_EXECUTABLE(TcpServer ${SRC_LIST})
