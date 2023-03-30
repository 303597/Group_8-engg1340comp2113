#include "game.h"
#include "characters.h"
#include "tools.h"
#include <chrono>
#include <string>
#include <vector>
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

    bool in_counteratk_mode = false;
    double ghost_speed = 0.4;
    PacMan pacman; vector<Ghost> ghosts;
    Map game_map = Map("../map/3_Monsters/map1.txt", pacman, ghosts);
    pacman.linkMap(&game_map);
    for (size_t i = 0; i < ghosts.size(); i++)
        ghosts[i].linkMap(&game_map);
    game_map.show();

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

        pacman.move(direction);
        int tile_info = game_map.updateTile(pacman.x, pacman.y);
        /*
        if (tile_info == 1)
            for (Ghost ghost: ghosts)
                ghost.in_counteratk_mode = true;
                */
        checkCharacterCollision(pacman, ghosts);

        for (Ghost ghost: ghosts)
            ghost.move(pacman.x, pacman.y, ghost_speed);
        checkCharacterCollision(pacman, ghosts);

        refresh();
        last_frame_time = this_frame_time;
    }
}