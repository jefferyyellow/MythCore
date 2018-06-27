cmake_minimum_required (VERSION 2.8)
PROJECT(GameServer)
#定义宏
ADD_DEFINITIONS(-DMYTH_OS_UNIX)
#包含的头文件目录
INCLUDE_DIRECTORIES(inc ../MythCore/include ../dep/tinyxml ../dep/mysql/include ../dep/protobuffer ../message ../dep ../dep/libcurl/include ../dep/libevent/include/linux ../dep/jsoncpp ../dep/hiredis/linux/include)
#设置C++的编译标志
SET(CMAKE_CXX_FLAGS "-g -pipe -Werror -DMEDIUM_SIZE -DMYTH_OS_UNIX -D__DEBUG__ -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'")

#指定可执行文件目录
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Debug ${CMAKE_CURRENT_SOURCE_DIR}/../dep/mysql/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/protobuffer/lib)
SET(EXECUTABLE_OUTPUT_PATH ../../Debug)

#连接MyCore和rt库
LINK_LIBRARIES(MythCore rt mysqlclient protobuf dl )
ADD_SUBDIRECTORY(src)
#message(STATUS "src list*****************************" ${MESSAGE_SRC_LIST})

ADD_SUBDIRECTORY(../message/ message)
#所有的文件名面前加上 src/(相对主CMakeLists的目录)
foreach(messagesrcfile ${MESSAGE_SRC_LIST})
	list(APPEND COMPLETE_MESSAGE_SRC_LIST ../message/${messagesrcfile})
endforeach(messagesrcfile)

SET(TINY_XML_SRC_LIST ../dep/tinyxml/tinyxml2.cpp)

#生成MythCore
ADD_EXECUTABLE(GameServer ${SRC_LIST} ${COMPLETE_MESSAGE_SRC_LIST} ${TINY_XML_SRC_LIST})
