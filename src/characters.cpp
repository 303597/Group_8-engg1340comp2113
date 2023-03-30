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
    // not implemented
    // note: check counteratk mode, handle eat results
}
