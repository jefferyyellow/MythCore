# $(call source-to-object, source-file-list)
source_to_object = $(subst .cpp,.o,$(filter %.cpp,$1))

# $(call make_library library_name,source_file_list)
define make_library
  $1 : $2
	ar r $$@ $$^
endef

# $(call one_compile_rules,source_file,object_file,cxxflags)
define one_compile_rules
  $2:$1
	$(CXX) -MM -MF $$(subst .o,.d,$$@) -MP -MT $$@ $(CFLAGS) $3 $(CPPFLAGS) $(TARGET_ARCH) $$<
	$(CXX) -c $(CFLAGS) $3 $(CPPFLAGS) -o $$@ $$<
endef


# $(call compile_rules,source_file_list,bin-directory,cxxflags)
define compile_rules
  $(foreach f,$1,$(eval $(call one_compile_rules,$f,$(addprefix $2,$(addsuffix .o,$(notdir $(basename $f)))),$3)))
endef

# $(call make_depend,source_file,object_file)
define make_depend
  echo $@
  $(CXX) -MM -MF $(subst .o,.d,$@) -MP -MT $2 $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $1
endef


# $(call clean_path,object_path)
define clean_path
  $(foreach f,$1,$(shell cd $f;rm -f *.d;rm -f *.o))
endef