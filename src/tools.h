#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>
#include "characters.h"

using namespace std;

//extern string username;
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
    int updateTile(int x, int y, vector<Ghost> ghosts, string &special, int &prop_turns); // return 0: no buff, 1: energizer
    void saveToFile(string filename);
    void readFromFile(string filename, PacMan &_pacman, vector<Ghost> &_ghosts);
    vector<string> vals;
    int total_num = 0;
    bool fl = false;
};

class Menu
{
private:
    vector<string> vals;
public:
    Menu(string filename);
    void showWelcome(int selected = -1);
    void showInGame(int score, int lives);
};

//void printMenuElement(int x, string line);

vector<pair<string, int>> getScoreRecords();

string getExecutablePath();

#endif
