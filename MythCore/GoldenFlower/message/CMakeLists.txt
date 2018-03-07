SET(TMP_MESSAGE_SRC_LIST
	common.hxx.pb.cc
	chatmodule.hxx.pb.cc
	dbmodule.hxx.pb.cc
	itemmodule.hxx.pb.cc
	loginmessage.hxx.pb.cc
	mapmodule.hxx.pb.cc
	message.hxx.pb.cc
	propertymodule.hxx.pb.cc
	skillmodule.hxx.pb.cc
	taskmodule.hxx.pb.cc
	template.hxx.pb.cc
)

#下面这一句要这么写主CMakeLists才能得到对应的值
SET(MESSAGE_SRC_LIST ${TMP_MESSAGE_SRC_LIST} CACHE INTERNAL ...)
#message(....${SRC_LIST})
