CXX = g++
CPPFLAGS =        # put pre-processor settings (-I, -D, etc) here
CXXFLAGS = -std=c++11 # put compiler settings here
LDFLAGS = -lboost_system # put linker settings here
RM = rm -f
OBJS = *.o

example: example.o SimpleSerial.o
	    g++ -o example example.o SimpleSerial.o $(LDFLAGS)

example.o: example.cpp
	    g++ -c example.cpp $(CXXFLAGS)

SimpleSerial.o: 
	    g++ -c SimpleSerial.cpp $(LDFLAGS) $(CXXFLAGS)

clean:
	$(RM) $(OBJS) example
