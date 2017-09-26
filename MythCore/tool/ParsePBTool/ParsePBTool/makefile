
COMMONPATH	= ../../../common
SERVERCOMMONPATH= ../../../server/servercommon

CC			= gcc
CXX			= g++
CFLAGS		= -g
CXXFLAGS	= -g -DLINUX -D_DEBUG_	 -DENCODING_UTF8
TINYFLAGS	= -DTIXML_USE_STL

INCPATH		= -I $(COMMONPATH)/mysql/inc/ -I $(COMMONPATH)/tiny -I $(COMMONPATH)/inc \
		  -I $(SERVERCOMMONPATH)/inc/ -I $(COMMONPATH)/protobuf/include -I./$(COMMONPATH)/include \
		  -I $(COMMONPATH)/log4cpp/include

LINK		= g++
LFLAGS		= 
LIBS		= -L$(COMMONPATH)/mysql/lib/ -lmysqlclient -L$(COMMONPATH)/protobuf/lib/ -lprotobuf -lpthread -lz -dl \
				 -L$(COMMONPATH)/lib -lgamecommonD  -lmsg -lpthread \
				-L$(COMMONPATH)/log4cpp/lib -llog4cpp 

DEL_FILE	= rm -f


OBJECTS		= ./debug/main.o ./debug/QueryResultMysql.o ./debug/Field.o ./debug/DatabaseMysql.o \
		  ./debug/ParseProtobuf.o ./debug/ProtoXMLManager.o ./debug/PBToolCommon.o ./debug/DBMsgFactory.o \
		  ./debug/tinystr.o ./debug/tinyxml.o ./debug/tinyxmlerror.o ./debug/tinyxmlparser.o \
		  ./debug/message_pb.hxx.pb.o ./debug/servermessage_pb.hxx.pb.o ./debug/coremessage_pb.hxx.pb.o \
		  ./debug/db_msg_pb.hxx.pb.o ./debug/property_pb.hxx.pb.o ./debug/log_msg_pb.hxx.pb.o \
		  ./debug/proxymessage_pb.hxx.pb.o ./debug/promessage_pb.hxx.pb.o ./debug/arenamessage_pb.hxx.pb.o \
		  ./debug/activitymessag_pb.hxx.pb.o ./debug/SevenKLayout.o
		  
TINYXML	=	./debug/tinystr.o ./debug/tinyxml.o ./debug/tinyxmlerror.o ./debug/tinyxmlparser.o
TARGET		= parsepbtool

first:	all

./debug/main.o : main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/QueryResultMysql.o : QueryResultMysql.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/Field.o : Field.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/DatabaseMysql.o : DatabaseMysql.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/ParseProtobuf.o : ParseProtobuf.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<
	
./debug/ProtoXMLManager.o : ProtoXMLManager.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

./debug/DBMsgFactory.o : DBMsgFactory.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

./debug/PBToolCommon.o : PBToolCommon.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

########## tiny xml ##########################
./debug/tinystr.o : $(COMMONPATH)/tiny/tinystr.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxml.o : $(COMMONPATH)/tiny/tinyxml.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlerror.o : $(COMMONPATH)/tiny/tinyxmlerror.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlparser.o : $(COMMONPATH)/tiny/tinyxmlparser.cpp
	$(CXX) -c $(CXXFLAGS) $(TINYFLAGS) $(INCPATH) -o $@ $<

########## end tiny xml ######################

########### message file #####################
./debug/message_pb.hxx.pb.o : $(COMMONPATH)/inc/message_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/servermessage_pb.hxx.pb.o : $(SERVERCOMMONPATH)/inc/servermessage_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/coremessage_pb.hxx.pb.o : $(COMMONPATH)/inc/coremessage_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/db_msg_pb.hxx.pb.o :$(SERVERCOMMONPATH)/inc/db_msg_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/property_pb.hxx.pb.o : $(COMMONPATH)/inc/property_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/log_msg_pb.hxx.pb.o : $(SERVERCOMMONPATH)/inc/log_msg_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/proxymessage_pb.hxx.pb.o : $(SERVERCOMMONPATH)/inc/proxymessage_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/promessage_pb.hxx.pb.o : $(COMMONPATH)/inc/promessage_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/arenamessage_pb.hxx.pb.o : $(COMMONPATH)/inc/arenamessage_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
./debug/activitymessag_pb.hxx.pb.o : $(COMMONPATH)/inc/activitymessag_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
########## end message file ##################

########## log4cpp ##################
./debug/SevenKLayout.o : $(COMMONPATH)/log4cpp/src/SevenKLayout.cpp
	$(CXX) -c  $(CXXFLAGS) $(INCPATH) -o $@ $<
########## end log4cpp ##################

tiny: $(TINYXML)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	
clean:
	rm -f ./debug/*.o
	rm -f $(TARGET)
