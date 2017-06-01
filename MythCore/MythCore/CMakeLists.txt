PROJECT(MYTHCORE)
ADD_DEFINITIONS(-DMYTH_OS_UNIX)
SET(CMAKE_CXX_FLAGS "-g -pipe -Werror -finput-charset='gbk' -fexec-charset='gbk' -fwide-exec-charset='unicode'")
#SET(EXECUTABLE_OUTPUT_PATH ../../)
ADD_SUBDIRECTORY(src bin)

