CXXFLAGS 	= -g
CXX 		= g++
INCPATH		= 

LINK 		= g++
LFLAGS 		= 
LIBS		= 

TARGET		= restarttool

OBJS = ./debug/main.o ./debug/EpollTcp.o ./debug/config.o ./debug/ServerRestartTime.o

$(TARGET) : $(OBJS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS)


./debug/main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

./debug/EpollTcp.o : EpollTcp.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

./debug/config.o : config.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

./debug/ServerRestartTime.o : ServerRestartTimer.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f ./debug/*.o
