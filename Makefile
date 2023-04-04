TARGET = bin/pacman
FLAGS = -I lib/ncurses-6.4/include -I lib/ncurses-6.4/include/ncursesw -L lib/ncurses-6.4/lib -pedantic-errors -std=c++17 -lncursesw

bin/tools.o: src/tools.cpp src/characters.h src/tools.h
	g++ -c $< $(FLAGS) -o $@ 

bin/characters.o: src/characters.cpp src/characters.h src/tools.h
	g++ -c $< $(FLAGS) -o $@

bin/game.o: src/game.cpp src/tools.h src/characters.h src/game.h
	g++ -c $< $(FLAGS) -o $@

bin/main.o: src/main.cpp src/tools.h src/characters.h src/game.h
	g++ -c $< $(FLAGS) -o $@

$(TARGET): bin/main.o bin/game.o bin/characters.o bin/tools.o
	g++ $^ -o $(TARGET) -static $(FLAGS) -ltinfow

clean:
	rm bin/tools.o bin/characters.o bin/game.o bin/main.o bin/pacman

.PHONY: clean