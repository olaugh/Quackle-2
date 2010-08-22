CXX = g++
OBJS = main.o anagrammer.o board.o layout.o test.o

test : $(OBJS)
	$(CXX) $(DEBUG) -o test $(OBJS)

main.o : main.cpp test.h
	$(CXX) $(DEBUG) -c main.cpp

anagrammer.o : anagrammer.cpp anagrammer.h constants.h types.h
	$(CXX) $(DEBUG) -c anagrammer.cpp

board.o : board.cpp board.h constants.h layout.h types.h
	$(CXX) $(DEBUG) -c board.cpp

layout.o : layout.cpp constants.h types.h layout.h
	$(CXX) $(DEBUG) -c layout.cpp

test.o : test.cpp anagrammer.h constants.h layout.h test.h types.h 
	$(CXX) $(DEBUG) -c test.cpp

clean :
	rm $(OBJS)

debug :
	$(MAKE) $(MAKEFILE) DEBUG="-g -D DEBUG"