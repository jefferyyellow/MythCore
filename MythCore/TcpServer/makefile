CXX		= g++
CXXFLAGS 	= -g -pipe -Werror -DMEDIUM_SIZE -DMYTH_OS_UNIX -D__DEBUG__ -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'
LINK     	= g++
LFLAGS   	= 

modules := ./
programs :=
libraries := 
object_path :=
server_sources :=


local_program := TcpServer

local_server_src := tcpserver.cpp main.cpp
loca_object_path :=./debug/

programs += $(local_program)
object_path += $(loca_object_path)
server_sources += $(local_server_src)

LIBS	= -lrt -lpthread -L../debug -lMythCore

server_objects += $(addprefix $(object_path),$(addsuffix .o,$(notdir $(basename $(server_sources)))))
dependencies = $(subst .o,.d,$(server_objects))

include_dirs = ./ ../dep/mysql/include ../MythCore/include
CPPFLAGS += $(addprefix -I,$(include_dirs))

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
