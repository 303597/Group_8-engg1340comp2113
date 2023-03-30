#ifndef TOOLS_H
#define TOOLS_H

#include "characters.h"
#include <vector>
#include <string>
using namespace std;

class Map
{
private:
    vector<string> vals;
    PacMan* pacman;
    vector<Ghost*> ghosts;
    void printMapElement(int x, int y, char element);

public:
    //Map(vector<string> _vals, PacMan* _pacman, vector<Ghost*> ghosts);
    Map::Map(string filename, PacMan &_pacman, vector<Ghost> &_ghosts);
    void show();
    bool isWall(int x, int y);
    int updateTile(int x, int y); // return 0: no buff, 1: energizer
};

//class Controller
//{
//
//};

#endif