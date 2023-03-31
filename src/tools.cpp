#include "tools.h"
#include "characters.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <libgen.h>
#include <unistd.h>
#include <linux/limits.h>
using namespace std;

Map::Map(string filename, PacMan &_pacman, vector<Ghost> &_ghosts) // _pacman: passed by reference  pacman: belongs to Map
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
        for (size_t j = 0; j < vals[i].length(); j++)
        {
            if (vals[i][j] == 'o')
            {
                pacman_count++;
                if (pacman_count != 1)
                {
                    cout << "ERROR: Map includes more than 1 pac-man." << endl;
                    return;
                }
                _pacman = PacMan(i, j);
                pacman = &_pacman;
                vals[i][j] = ' ';
            }
            if (vals[i][j] == 'E')
            {
                ghost_count++;
                _ghosts.emplace_back(Ghost(i, j));
                vals[i][j] = ' ';
                //ghosts.emplace_back(&_ghosts.back());
                //ghosts.emplace_back(new Ghost(i, j, this));
            }
        }
    ghosts = &_ghosts;
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
        {
            printMapElement(i, j, vals[i][j]);
        }
    }
    printMapElement(pacman->x, pacman->y, 'o');
    for (Ghost ghost: *ghosts)
        if (ghost.in_counteratk_mode)
            printMapElement(ghost.x, ghost.y, 'e');
        else
            printMapElement(ghost.x, ghost.y, 'E');
    refresh();
}

void Map::printMapElement(int x, int y, char element)
{
    switch (element)
    {
        case 'o':
            mvprintw(x, 2 * y, "🟡");
            break;
        case '#':
            mvprintw(x, 2 * y, "🟦");
            break;
        case '.':
            mvprintw(x, 2 * y, "◽");
            break;
        case 'E':mvprintw(x, 2 * y, "👻");
            break;
        case 'e':
            mvprintw(x, 2 * y, "🥶");
            break;
        case '0':
            mvprintw(x, 2 * y, "🟢");
            break;
        default:
            mvprintw(x, 2 * y, "ㅤ");
    }
}

int Map::updateTile(int x, int y)
{
    if (vals[x][y] == '0')
    {
        return 1;
    }
    else
    {
        if (vals[x][y] == '.')
        {
            // score++
        }
        vals[x][y] = ' ';
        return 0;
    }
    
    return 0;
}

string getPath() // note: this solution to get executable file path is found at https://stackoverflow.com/questions/8579065/c-c-executable-path
{
    char buf[PATH_MAX + 1];
    if (readlink("/proc/self/exe", buf, sizeof(buf) - 1) == -1)
        throw string("readlink() failed");
    string str(buf);
    return str.substr(0, str.rfind('/'));
}