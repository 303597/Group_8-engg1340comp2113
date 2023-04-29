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
    //Map(vector<string> _vals, PacMan &_pacman, vector<Ghost> &ghosts);
    Map(string filename, PacMan &_pacman, vector<Ghost> &_ghosts, bool from_saved_data = false); // TO BE MODIFIED
    void show(); // TO BE CALLED BY GAME
    bool isWall(int x, int y);
    int updateTile(int x, int y, vector<Ghost> ghosts, string &special, int &prop_turns); // TO BE DELETED // return 0: no buff, 1: energizer
    void saveToFile(string filename); // TO BE DELETED
    //static Map readFromFile(string filename, PacMan &_pacman, vector<Ghost> &_ghosts);
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

int read_line(string line);

void clearSavedData(string filename);

string getExecutablePath();

#endif
