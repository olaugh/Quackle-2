OBJS = main.o anagrammer.o board.o

test : $(OBJS)
	g++ -o test $(OBJS)

main.o : main.cpp
	g++ -c main.cpp

anagrammer.o : anagrammer.cpp anagrammer.h types.h
	g++ -c anagrammer.cpp

board.o : board.cpp board.h
	g++ -c board.cpp

clean :
	rm $(OBJS)