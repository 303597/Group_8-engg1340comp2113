#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <vector>
#include "tools.h"
using namespace std;

class Map;

class Character
{
public:
    Character(int start_x = 0, int start_y = 0);
    void linkMap(Map* _map);
    int start_x, start_y; // initial position
    int x, y; // present position
    //bool in_counteratk_mode = false; // whether in counter-attack mode

protected:
    Map* map;
};

class PacMan: public Character
{
public:
    using Character::Character;
    void move(int direction, string special);
    void show(string special = "none", bool in_counteratk_mode = false);
    int lives = 2;
    int eaten_ghosts = 0, expression_timer = 0, expression_index = 0;
private:
    const string expression_list[5] = {"😉", "😶", "😏", "😑", "😄"};
};

class Ghost: public Character
{
public:
    using Character::Character;
    void move(int target_x, int target_y, int ghost_num);
    void show();
    bool moveBFS(int target_x, int target_y);
    bool in_counteratk_mode = false;
    int type = 0;
    double speed = 0.0;
};

//void checkCharacterCollision(PacMan &pacman, vector<Ghost> &ghosts,int &turns, int &direction);
//void checkCharacterCollision(PacMan &pacman, vector<Ghost> &ghosts,int &turns, int &direction, int &prop_lasting_time, int &fruit_lasting_time, int &prop_turns, string &special); 

#endif
