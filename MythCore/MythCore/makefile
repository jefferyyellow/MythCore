# $(call source-to-object, source-file-list)
source_to_object = $(subst .cpp,.o,$(filter %.cpp,$1))

#$(subdirectory)
subdirectory=$(patsubst %/module.mk,%,$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))


# $(call make_library library_name,source_file_list)
define make_library
  libraries += $1
  $1:$2
	$(AR) $(ARFLAGS) $$@ $$^
endef

# $(call one_compile_rules,source_file,object_file)
define one_compile_rules
  $2:$1
	g++ -MM -MF $$(subst .o,.d,$$@) -MP -MT $$@ $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $$<
	$(COMPILE.cpp) $(OUTPUT_OPTION) $$@ $$<
endef


# $(call compile_rules,source_file_list,bin-directory)
define compile_rules
  $(foreach f,$1,$(eval $(call one_compile_rules,$f,$(addprefix $2,$(addsuffix .o,$(notdir $(basename $f)))))))
endef

CXX		= g++
CXXFLAGS 	= -g   -pipe -Werror -DMYTH_OS_UNIX -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'

modules := ../MythCore
programs :=
libraries :=
object_path :=
server_sources :=

server_objects += $(addprefix $(object_path),$(addsuffix .o,$(notdir $(basename $(server_sources)))))
dependencies = $(subst .o,.d,$(server_objects))

include_dirs = /usr/local/gcc-4.3.3/include/c++/4.3.3 ./include 
CPPFLAGS += $(addprefix -I,$(include_dirs))

vpath %.h $(include_dirs)

MV := mv -f
RM := rm -f
SED:= sed

all:

include $(addsuffix /module.mk,$(modules))

.PHONY :all

all:$(programs)


.PHONY : libraries
libraries : $(libraries)

.PHONY : clean
clean:
	$(RM) $(programs) $(libraries) 
	$(call clean_path,$(object_path))

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(dependencies)
endif

# $(call make_depend,source_file,object_file)
define make_depend
  echo $@
  g++ -MM -MF $(subst .o,.d,$@) -MP -MT $2 $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $1
endef


# $(call clean_path,object_path)
define clean_path
  $(foreach f,$1,$(shell cd $f;rm -f *.d;rm -f *.o))
endef

#$(eval $(call compile_rules,$(sources),$(object_path)))

#%.d:%.cpp
#	g++ $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< | \
#	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
#	$(MV) $@.tmp $@
