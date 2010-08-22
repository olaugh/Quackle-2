CXX = g++
OBJS = main.o anagrammer.o board.o layout.o move.o test.o util.o

test : $(OBJS)
	$(CXX) $(DEBUG) -o test $(OBJS)

main.o : main.cpp test.h
	$(CXX) $(DEBUG) -c main.cpp

anagrammer.o : anagrammer.cpp anagrammer.h board.h constants.h move.h types.h
	$(CXX) $(DEBUG) -c anagrammer.cpp

board.o : board.cpp board.h constants.h layout.h types.h
	$(CXX) $(DEBUG) -c board.cpp

layout.o : layout.cpp constants.h types.h layout.h
	$(CXX) $(DEBUG) -c layout.cpp

move.o : move.cpp constants.h types.h move.h util.h
	$(CXX) $(DEBUG) -c move.cpp

test.o : test.cpp anagrammer.h constants.h layout.h test.h types.h 
	$(CXX) $(DEBUG) -c test.cpp

clean :
	rm $(OBJS)

debug :
	$(MAKE) $(MAKEFILE) DEBUG="-g -D DEBUG"