#include "game.h"
#include <chrono>
#include <string>
#include <iostream>
#include <ncurses.h>
using namespace std;

//void save() //high score or map

//void UI() //welcome, gamescore, end, user interfaces

void welcomeLoop()
{
    // not implemented, to be done with UI
}

void gameLoop()
{
    nodelay(stdscr, true); // don't wait until input

    int dirx[] = {0, 0, -1, 1};
    int diry[] = {-1, 1, 0, 0}; // up, down, left, right
    auto last_frame_time = chrono::high_resolution_clock::now(), this_frame_time = last_frame_time;

    int direction = 0;
    while (true)
    {
        int operation = 0;
        while (chrono::duration_cast<chrono::milliseconds>(this_frame_time - last_frame_time).count() < time_per_loop)
        {
            int ch = getch();
            if (ch != ERR)
                operation = ch;
            this_frame_time = chrono::high_resolution_clock::now();
        }
        switch (operation)
        {
            case 'w':
            case KEY_UP:
                direction = 0;
                break;
            case 's':
            case KEY_DOWN:
                direction = 1;
                break;
            case 'a':
            case KEY_LEFT:
                direction = 2;
                break;
            case 'd':
            case KEY_RIGHT:
                direction = 3;
                break;
        }

        mvprintw(0, 0, "%d", direction);
        refresh();
        last_frame_time = this_frame_time;
    }
}