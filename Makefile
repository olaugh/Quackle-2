CXX = g++
FLAGS = -std=gnu++0x -m64
OBJS = main.o anagrammer.o board.o config.o move.o rack.o test.o util.o

test : $(OBJS)
	$(CXX) $(FLAGS) $(DEBUG) -o test $(OBJS)

main.o : main.cpp test.h
	$(CXX) $(FLAGS) $(DEBUG) -c main.cpp

anagrammer.o : anagrammer.cpp anagrammer.h board.h constants.h move.h types.h
	$(CXX) $(FLAGS) $(DEBUG) -c anagrammer.cpp

board.o : board.cpp board.h config.h constants.h types.h
	$(CXX) $(FLAGS) $(DEBUG) -c board.cpp

config.o : config.cpp constants.h types.h config.h
	$(CXX) $(FLAGS) $(DEBUG) -c config.cpp

move.o : move.cpp constants.h types.h move.h util.h
	$(CXX) $(FLAGS) $(DEBUG) -c move.cpp

rack.o : rack.cpp constants.h types.h rack.h util.h
	$(CXX) $(FLAGS) $(DEBUG) -c rack.cpp

test.o : test.cpp anagrammer.h config.h constants.h rack.h test.h types.h 
	$(CXX) $(FLAGS) $(DEBUG) -c test.cpp

clean :
	rm $(OBJS)

debug :
	$(MAKE) $(MAKEFILE) DEBUG="-g -D DEBUG"