#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "tools.h"

class Character
{
public:
    Character(int start_x, int start_y, Map* on_map);
    int x, y;
    int mode;
    Map* map;
};

class PacMan: public Character
{
public:
    void move(int direction);
    int lives = 3;
};

class Ghost: public Character
{
public:
    void move(int target_x, int target_y, bool in_counteratk_mode);
};

void checkCollision(PacMan* pacman, vector<Ghost*> ghosts);

#endif
