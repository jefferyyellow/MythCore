SET(TMP_SRC_LIST
	log.cpp
	logdisplayer.cpp
	logmanager.cpp
	fileutility.cpp
	timemanager.cpp
	performance.cpp
	i18n.cpp
	epollmodel.cpp
	tcpsocket.cpp
	selectmodel.cpp
	linuxthread.cpp
	winthread.cpp
	simplelock.cpp
	threadpool.cpp
	mysqlqueryresult.cpp
	mysqldatabase.cpp
	sharememory.cpp
	socketstream.cpp
)

#所有的文件名面前加上 src/(相对主CMakeLists的目录)
foreach(filename ${TMP_SRC_LIST})
	list(APPEND COMPLETE_SRC_LIST src/${filename})
endforeach(filename)

#下面这一句要这么写主CMakeLists才能得到对应的值
SET(SRC_LIST ${COMPLETE_SRC_LIST} CACHE INTERNAL ...)
#message(....${SRC_LIST})
