#include "tools.h"
#include "game.h"
#include "characters.h"
#include <locale.h>
#include <ncurses.h>
#include <filesystem>
#include <string>
#include <ctime>

bool start_game = true;

void initialize()
{
    setenv("TERMINFO", "/lib/terminfo", 1);
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    noecho();
    keypad(stdscr, true);
    srand(time(0));
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
