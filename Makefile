CXX = g++
CPPFLAGS =        # put pre-processor settings (-I, -D, etc) here
CXXFLAGS = -std=c++11 # put compiler settings here
LDFLAGS = -lboost_system # put linker settings here
RM = rm -f

example: example.o SimpleSerial.o
	    g++ -o example example.o SimpleSerial.o $(LDFLAGS)

example.o: example.cpp
	    g++ -c example.cpp -std=c++11

SimpleSerial.o: 
	    g++ -c SimpleSerial.cpp -lboost_system -std=c++11

clean:
	$(RM) *.o example
