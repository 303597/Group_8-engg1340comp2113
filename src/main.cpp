//#include "tools.h"
#include "game.h"
//#include "characters.h"
#include <locale.h>
#include <ncurses.h>

bool start_game = true;

void initialize()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    keypad(stdscr, true);

    //Map game_map = Map("../map/3_Monsters/map1.txt");

    //game_map.show();
}

int main()
{
    initialize();

    //welcomeLoop();

    if (start_game)
        gameLoop();

    //showHighScore();

    getch();
    endwin();

    return 0;
}
