#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>
using namespace std;

class Map
{
private:
    vector<string> vals;
    //PacMan* pacman;
    //Ghost[3]* ghosts;
    void printMapElement(int x, int y, char element);

public:
    Map(vector<string> _vals);
    Map(string filename);
    void show();
};

//class Controller
//{
//
//};

#endif