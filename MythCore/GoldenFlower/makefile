CXX		= g++
CXXFLAGS 	= -g -pipe -Werror -DMEDIUM_SIZE -DMYTH_OS_UNIX -D__DEBUG__ -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'
LINK     	= g++
LFLAGS   	= 

modules := ./
programs :=
libraries := 
object_path :=
server_sources :=


local_program := gameserver
local_server_src := main.cpp dbjob.cpp entity.cpp gameserver.cpp gameserverconfig.cpp internalmsg.cpp internalmsgpool.cpp job.cpp \
locallogjob.cpp loginjob.cpp loginmodule.cpp messagefactory.cpp objmanager.cpp objpool.cpp scenejob.cpp
local_message_src := loginmessage.hxx.pb.cc message.hxx.pb.cc
loca_object_path :=./debug/
LIBS	= -lrt -ldl -lpthread -L../debug -lMythCore -L../dep/mysql/lib -lmysqlclient -L../dep/protobuffer/lib -lprotobuf

include_dirs = ./inc ../dep/mysql/include ../MythCore/include ./tinyxml ../message ../dep/protobuffer
CPPFLAGS += $(addprefix -I,$(include_dirs))

programs += $(local_program)
object_path += $(loca_object_path)
server_sources += $(addprefix src/,$(local_server_src))
server_sources += tinyxml/tinyxml2.cpp
server_sources += $(addprefix ../message/, $(local_message_src))



server_objects += $(addprefix $(object_path),$(addsuffix .o,$(notdir $(basename $(server_sources)))))
dependencies = $(subst .o,.d,$(server_objects))



vpath %.h $(include_dirs)

MV := mv -f
RM := rm -f
SED:= sed

include $(addsuffix /module.mk,$(modules))


.PHONY :all
all:$(programs)

$(eval $(call compile_rules,$(server_sources),$(object_path),$(CXXFLAGS)))

$(local_program): $(server_objects) $(libraries)
	$(LINK) $(LFLAGS) -o $(local_program) $(server_objects) $(LIBS) $(libraries)

.PHONY : libraries
libraries : $(libraries)

.PHONY : clean
clean:
	$(RM) $(programs) $(libraries) 
	$(call clean_path,$(object_path))

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(dependencies)
endif
