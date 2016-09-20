CXX		= distcc g++
CXXFLAGS 	= -g  -fno-inline -pipe -Werror -DMYTH_OS_UNIX -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'

LINK     	= g++
LFLAGS   	= 
module :=
programs := main
libraries :=
object_path :=
#sources :=
server_sources :=
msglib_sources :=

include module.mk

include_dirs = ../MythCore/include

CPPFLAGS += $(addprefix -I,$(include_dirs))

vpath %.h $(include_dirs)

MV := mv -f
RM := rm -f
SED:= sed

LIBS_BASE	= -lrt
LIBS_DEBUG	= -L../MythCore -lMythCore


LIBCOMMON=
debug:LIBS=${LIBS_DEBUG} ${LIBS_BASE} 
debug:LIBCOMMON=libMythCore.a
debug: all 

local_server_src := main.cpp
local_src := $(addprefix src/,$(local_server_src))

loca_object_path := debug/

object_path += $(loca_object_path)

server_sources += $(local_src)

server_objects += $(addprefix $(object_path),$(addsuffix .o,$(notdir $(basename $(server_sources)))))

#PCH_X_H = stdafx.hpp
#PCH_X = stdafx.hpp.gch
#$(server_objects): $(PCH_X)

#$(PCH_X): $(PCH_X_H)
#	g++ $(CXXFLAGS) $(CPPFLAGS) -x c++-header $> $^

$(programs): $(server_objects) $(libraries)
	$(LINK) $(LFLAGS) -o $(programs) $(server_objects) $(LIBS) $(libraries)


$(eval $(call compile_rules,$(server_sources),$(object_path),$(CXXFLAGS)))

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(dependencies)
endif

all:
.PHONY :all

all:$(programs)

.PHONY : clean
clean:
	$(RM) $(programs)
	$(call clean_path,$(object_path))
	rm -f $(PCH_X)
