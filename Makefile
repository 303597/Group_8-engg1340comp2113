TARGET = pacman
FLAGS = -pedantic-errors -std=c++17 -lncursesw

tools.o: src/tools.cpp src/characters.h src/tools.h
	g++ $(FLAGS) -c $<

game.o: src/game.cpp src/tools.h src/characters.h src/game.h
	g++ $(FLAGS) -c $<

main.o: src/main.cpp src/tools.h src/characters.h src/game.h
	g++ $(FLAGS) -c $<

$(TARGET): main.o tools.o game.o
	g++ $(FLAGS) $^ -o $(TARGET)