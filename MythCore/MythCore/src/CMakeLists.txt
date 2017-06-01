INCLUDE_DIRECTORIES(../include ../../dep/mysql/include)
ADD_LIBRARY(MythCore STATIC log.cpp logdisplayer.cpp logmanager.cpp fileutility.cpp timemanager.cpp performance.cpp i18n.cpp 
	epollmodel.cpp tcpsocket.cpp selectmodel.cpp linuxthread.cpp winthread.cpp simplelock.cpp threadpool.cpp 
	mysqlqueryresult.cpp mysqldatabase.cpp sharememory.cpp socketstream.cpp)
