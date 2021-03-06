cmake_minimum_required (VERSION 2.8)
PROJECT(ServerCrashResave)
#定义宏
ADD_DEFINITIONS(-DMYTH_OS_UNIX)
#包含的头文件目录
INCLUDE_DIRECTORIES(../GameServer/inc ../MythCore/include ../dep/tinyxml ../dep/mysql/include ../dep/protobuffer ../message ../dep ../dep/libcurl/include ../dep/libevent/linux/include/ ../dep/jsoncpp ../dep/hiredis/linux/include ../dep/gtest/include)
#设置C++的编译标志
SET(CMAKE_CXX_FLAGS "-g -pipe  -fprofile-arcs -ftest-coverage -DMEDIUM_SIZE -DMYTH_OS_UNIX -D__DEBUG__ -D__SERVER_CRASH_RESAVE__ -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'")

#指定可执行文件目录
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Debug ${CMAKE_CURRENT_SOURCE_DIR}/../dep/mysql/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/protobuffer/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/gtest/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/hiredis/linux/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/jsoncpp/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/libcurl/lib ${CMAKE_CURRENT_SOURCE_DIR}/../dep/libevent/linux/lib)
SET(EXECUTABLE_OUTPUT_PATH ../Debug)

#连接MyCore和rt库
#idn ldap ssl这些库都是因为curl库链接需要
LINK_LIBRARIES(MythCore rt mysqlclient protobuf dl gtest hiredis curl event_core idn ldap ssl pthread jsoncpp)
ADD_SUBDIRECTORY(../GameServer/src gamesrc)
#message(STATUS "src list*****************************" ${MESSAGE_SRC_LIST})

ADD_SUBDIRECTORY(../message/ message)
#所有的文件名面前加上 src/(相对主CMakeLists的目录)
foreach(messagesrcfile ${MESSAGE_SRC_LIST})
	list(APPEND COMPLETE_MESSAGE_SRC_LIST ../message/${messagesrcfile})
endforeach(messagesrcfile)

foreach(srcfile ${SRC_LIST})
	list(APPEND GAME_SERVER_SRC_LIST ../GameServer/${srcfile})
endforeach(srcfile)

SET(TINY_XML_SRC_LIST ../dep/tinyxml/tinyxml2.cpp)
SET(MAIN_SRC main.cpp ServerResave.cpp)

#生成MythCore
ADD_EXECUTABLE(ServerCrashResave ${MAIN_SRC} ${GAME_SERVER_SRC_LIST} ${COMPLETE_MESSAGE_SRC_LIST} ${TINY_XML_SRC_LIST})
