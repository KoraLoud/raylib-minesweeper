# make minesweeper-raylib

minesweeper:	main.o
	g++ main.o -Wall -I~/raylib/src -L~/raylib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o minesweeper

main.o:	main.cpp
	g++ -c main.cpp -I~/raylib/src

clean:
	/bin/rm -f minesweeper *.o