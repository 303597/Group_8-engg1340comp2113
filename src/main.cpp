#include "tools.h"
#include "game.h"
#include "characters.h"
#include <locale.h>
#include <cursesw.h>
#include <filesystem>
#include <string>
#include <ctime>

void initialize()
{
    setenv("TERMINFO", "/lib/terminfo", 1);
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    srand(time(0));
}

int main()
{
    initialize();   

    while (true)
    {
        int operation = welcomeLoop();

        if (operation == 0);
            // Not implemented
        if (operation == 1)
            gameLoop(1);
        
        if (operation == 2)
            showTutorial();

        if (operation == 3)
            showHighScore();
        
        if (operation == 4)
            break;
    }

    endwin();

    return 0;
}
