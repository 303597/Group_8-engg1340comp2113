#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <filesystem>
#include "tools.h"
#include "characters.h"
#include "game.h"
using namespace std;

//void save() //high score or map

//void UI() //welcome, gamescore, end, user interfaces

void welcomeLoop()
{
    // not implemented, to be done with UI
}

bool gameLoop()
{
    nodelay(stdscr, true); // don't wait until input

    int score = 0;

    bool in_counteratk_mode = false;
    int turns = 0;
    double ghost_speed = 0.4;
    PacMan pacman; vector<Ghost> ghosts;

    
    string map_path = getPath() + "/../map";
    Map game_map = Map(map_path + "/2_Monsters/map2.txt", pacman, ghosts);
    pacman.linkMap(&game_map);
    for (size_t i = 0; i < ghosts.size(); i++)
        ghosts[i].linkMap(&game_map);
    game_map.show();

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
        if (tile_info == 1)
        {
            turns++;      
            for (Ghost &ghost: ghosts)
            {
                ghost.in_counteratk_mode = true;
            }
        }
        if(turns == ghosts.size() * 15){
            turns = 0;
            for (Ghost &ghost: ghosts)
            {
                ghost.in_counteratk_mode = false;
            }
        }
        if(turns != 0){
            turns++;
        }
        
        checkCharacterCollision(pacman, ghosts);

        for (Ghost &ghost: ghosts)
            ghost.move(pacman.x, pacman.y, ghost_speed);
        checkCharacterCollision(pacman, ghosts);
        
	    refresh();

        game_map.show();

        refresh();
        last_frame_time = this_frame_time;
        mvprintw(0, 0, "🟦"); // move cursor
        if (pacman.lives <= 0)
            return false;
    }
}
