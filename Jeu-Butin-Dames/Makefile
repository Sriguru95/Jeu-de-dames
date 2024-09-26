# compilateur + options
CC = g++ -Wall -std=c++11
CCO = $(CC) -Isrc/include -c $<

# liste des objets intermédiaires
OBJECTS = main.o Menu.o Pawn.o Square.o Board.o Player.o ButinGame.o DamesGame.o

all : compile link

compile: $(OBJECTS)

link:
	$(CC) $(OBJECTS) -o main.exe -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

main.o : main.cpp Menu.hpp 
	$(CCO)

Menu.o : Menu.cpp Menu.hpp
	$(CCO)

Pawn.o : Pawn.cpp Pawn.hpp
	$(CCO)

Square.o : Square.cpp Square.hpp
	$(CCO)

Board.o : Board.cpp Board.hpp 
	$(CCO)

Player.o : Player.cpp player.hpp 
	$(CCO)

ButinGame.o : ButinGame.cpp ButinGame.hpp
	$(CCO)

DamesGame.o : DamesGame.cpp DamesGame.hpp
	$(CCO)

clean :
	rm -f *.o main.exe
