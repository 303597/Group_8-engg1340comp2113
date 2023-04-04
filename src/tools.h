#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>
#include "characters.h"

using namespace std;

extern int score;
extern int dirx[]; //  = {0, 0, -1, 1}
extern int diry[]; // = {-1, 1, 0, 0}// up, down, left, right

class Character;

class PacMan;

class Ghost;

class Map
{
private:
    PacMan* pacman;
    vector<Ghost>* ghosts;
    void printMapElement(int x, int y, char element);

public:
    //Map(vector<string> _vals, PacMan* _pacman, vector<Ghost*> ghosts);
    Map(string filename, PacMan &_pacman, vector<Ghost> &_ghosts);
    void show();
    bool isWall(int x, int y);
    int updateTile(int x, int y); // return 0: no buff, 1: energizer
    vector<string> vals;
};

string getExecutablePath();

#endif