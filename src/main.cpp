#include "tools.h"
#include "game.h"
#include "characters.h"
#include <locale.h>
#include <ncurses.h>
#include <filesystem>
#include <string>

bool start_game = true;

void initialize()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    keypad(stdscr, true);
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
