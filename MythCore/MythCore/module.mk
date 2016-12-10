local_program := 

local_server_src := log.cpp logdisplayer.cpp logmanager.cpp fileutility.cpp timemanager.cpp performance.cpp i18n.cpp epollmodel.cpp tcpsocket.cpp \
					selectmodel.cpp linuxthread.cpp winthread.cpp simplelock.cpp threadpool.cpp mysqlqueryresult.cpp mysqldatabase.cpp
local_src := $(addprefix src/,$(local_server_src))
loca_object_path := $(subdirectory)/debug/

programs += $(local_program)
object_path += $(loca_object_path)
server_sources += $(local_src)

LIBS_BASE	= -L/usr/local/gcc-4.3.3/lib -lrt -lpthread


$(eval $(call make_library,libMythCore.a,$(server_objects)))

$(eval $(call compile_rules,$(server_sources),$(object_path)))
