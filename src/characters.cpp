#include "tools.h"
#include "characters.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

Character::Character(int _start_x, int _start_y)
{
    start_x = x = _start_x; start_y = y = _start_y;
}

void Character::linkMap(Map* _map)
{
    map = _map;
}

void PacMan::move(int direction)
{
    // not implemented
    // note: check collision
    
    int x_temp, y_temp;
    x_temp = x + dirx[direction]; y_temp = y + dirx[direction];

    if (map -> vals[x_temp][y_temp] != '#')
    {
        x = x + dirx[direction]; y = y + dirx[direction];
    }
    
}

void Ghost::move(int target_x, int target_y, double speed)
{
    // not implemented
}

void checkCharacterCollision(PacMan pacman, vector<Ghost> ghosts)
{   
    for (int i=0;i<ghosts.size();++i)
    {
        if (ghosts[i].x==pacman.x && ghosts[i].y==pacman.y)
        {
            // determine the mode of the ghost
            if (ghosts[i].in_counteratk_mode)
            {
                pacman.eaten_ghosts++;
                score += 50*pacman.eaten_ghosts;
                ghosts[i].x = ghosts[i].start_x;
                ghosts[i].y = ghosts[i].start_y;
                // the eaten ghost reset
            }
            else
            {
                pacman.lives--;
                if (pacman.lives == 0) return; // player dies
                pacman.x = pacman.start_x;
                pacman.y = pacman.start_y;
                for (int j=0; j<ghosts.size(); ++j)
                {
                    ghosts[j].x = ghosts[j].start_x;
                    ghosts[j].y = ghosts[j].start_y;
                }
                // player and all the ghosts reset
            }
        }
    }
    // note: check counteratk mode, handle eat results
}
