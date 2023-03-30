#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "tools.h"

class Character
{
public:
    Character(int start_x = 0, int start_y = 0);
    void linkMap(Map* _map);
    //int start_x, start_y; // initial position
    int x, y; // present position
    //bool in_counteratk_mode = false; // whether in counter-attack mode

private:
    Map* map;
};

class PacMan: public Character
{
public:
    using Character::Character;
    void move(int direction);
    int lives = 3;
};

class Ghost: public Character
{
public:
    using Character::Character;
    void move(int target_x, int target_y, double ghost_speed);
    bool in_counteratk_mode = false;
    double steps = 0;
};

bool checkCharacterCollision(PacMan pacman, vector<Ghost> ghosts);

#endif
