#include "tools.h"
#include "game.h"
#include "characters.h"
#include <locale.h>
#include <ncurses.h>

const double fps = 60.00;

bool start_game = false;

void initialize()
{
    
    initscr();
}

int main()
{
    initialize();

    welcomeLoop();

    if (start_game)
        gameLoop();

    showHighScore();

    return 0;
}