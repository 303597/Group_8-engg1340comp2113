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
//private:
    //PacMan* pacman;
    //vector<Ghost>* ghosts;
    //void printMapElement(int x, int y, char element);

public:
    Map(vector<string> _vals);
    void show();
    bool isWall(int x, int y);
    int updateTile(int x, int y, vector<Ghost*> ghosts, string &special, int &prop_turns); // return 0: no buff, 1: energizer
    //void saveToFile(string filename);
    //static Map readFromFile(string filename, PacMan &_pacman, vector<Ghost> &_ghosts);
    vector<string> vals;
    int total_num = 0;
    bool fl = false;
    string& operator[](int index);
    size_t size();
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

struct ScoreRecord
{
    string username;
    int score, level;
    bool operator<(ScoreRecord &x)
    {
        if (score == x.score)
            return level < x.level;
        return score < x.score;
    }
};

//void printMenuElement(int x, string line);

vector<ScoreRecord> getScoreRecords();

int read_line(string line);

void clearSavedData(string filename);

string getExecutablePath();

void printElement(int x, int y, char element);

#endif
