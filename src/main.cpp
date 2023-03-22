#include "tools.h"
//#include "game.h"
//#include "characters.h"
#include <locale.h>
#include <ncurses.h>

const double fps = 60.00;

bool start_game = false;

void initialize()
{
    setlocale(LC_ALL, "");
    initscr();

    Map game_map = Map("../map/3_Monsters/map1.txt");

    game_map.show();
}

int main()
{
    initialize();

    //welcomeLoop();

    //if (start_game)
    //    gameLoop();

    //showHighScore();

    getch();
    endwin();

    return 0;
}
