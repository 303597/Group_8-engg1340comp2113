#include "tools.h"
#include "characters.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

Map::Map(string filename)
{
    ifstream fin;
    fin.open(filename);
    if (fin.fail())
    {
        cout << "Failed to open file: " << filename << endl;
        return;
    }
    string str;
    while (getline(fin, str))
        vals.emplace_back(str);
    
    int pacman_count = 0, ghost_count = 0;
    for (size_t i = 0; i < vals.size(); i++)
        for (size_t j = 0; j < str.length(); j++)
        {
            if (vals[i][j] == 'o')
            {
                pacman_count++;
                if (pacman_count != 1)
                {
                    cout << "ERROR: Map includes more than 1 pac-man." << endl;
                    return;
                }
                //pacman = new PacMan(i, j, this);
                vals[i][j] = ' ';
            }
            if (vals[i][j] == 'E')
            {
                ghost_count++;
                //ghosts.emplace_back(new Ghost(i, j, this));
            }
        }
    
    if (pacman_count < 1)
        cout << "ERROR: Map does not include pac-man starting point." << endl;
}
//Map::Map(vector<string> _vals)
//{
//    vals = _vals;
//}

void Map::show()
{
    for (size_t i = 0; i < vals.size(); i++)
    {
        for (size_t j = 0; j < vals[i].length(); j++)
            printMapElement(i, j, vals[i][j]);
    }
    refresh();
}

void Map::printMapElement(int x, int y, char element)
{
    switch (element)
    {
        case 'o':
            mvprintw(x, y, "🟡");
            break;
        case '#':
            mvprintw(x, y, "🟦");
            break;
        case '.':
            mvprintw(x, y, "◽");
            break;
        case 'E':
            mvprintw(x, y, "👻");
            break;
        default:
            mvprintw(x, y, "ㅤ");
    }
}
