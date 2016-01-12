local_program := $(subdirectory)/server

local_server_src := log.cpp logdisplayer.cpp main.cpp logmanager.cpp fileutility.cpp timemanager.cpp performance.cpp i18n.cpp epollmodel.cpp tcpsocket.cpp selectmodel.cpp

local_src := $(addprefix src/,$(local_server_src))
loca_object_path := $(subdirectory)/debug/

programs += $(local_program)
object_path += $(loca_object_path)
server_sources += $(local_src)

LIBS_BASE	= -L/usr/local/gcc-4.3.3/lib -lrt

#PCH_X_H = inc/stdafx.hpp
#PCH_X = inc/stdafx.hpp.gch
#$(objects): $(PCH_X)

#$(PCH_X): $(PCH_X_H)

$(local_program): $(server_objects) $(libraries)
	$(LINK.cc) $^  -o $@ $(LIBS_BASE)


$(eval $(call compile_rules,$(server_sources),$(object_path)))
