TARGET = pacman
FLAGS = -pedantic-errors -std=c++17 -lncursesw

tools.o: tools.cpp tools.h
	g++ $(FLAGS) -c $<

game.o: game.cpp tools.h characters.h game.h
	g++ $(FLAGS) -c $<

main.o: main.cpp tools.h characters.h game.h
	g++ $(FLAGS) -c $<

$(TARGET): main.o tools.o games.o
	g++ $(FLAGS) $^ -o $(TARGET)