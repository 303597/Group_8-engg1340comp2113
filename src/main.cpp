#include "tools.h"
#include "game.h"
#include "characters.h"
#include <locale.h>
#include <ncurses.h>

bool start_game = true;

void initialize()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    keypad(stdscr, true);

    const int dirx[] = {0, 0, -1, 1};
    const int diry[] = {-1, 1, 0, 0};
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
