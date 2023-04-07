#include "tools.h"
#include "characters.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
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
        case '_':
            mvprintw(x, 2 * y, "🟩");
            break;
        case '.':
            mvprintw(x, 2 * y, "◽");
            break;
        case 'E':
            mvprintw(x, 2 * y, "👻");
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
        vals[x][y] = ' ';
        return 1;
    }
    else if(vals[x][y] == '.')
    {
        score += 2;
        vals[x][y] = ' ';
        return 0;
    }
    
    return 2;
}

string getExecutablePath() // This solution to get the path of the execuatable is found at https://stackoverflow.com/questions/1528298/get-path-of-executable
{
    string str = filesystem::canonical("/proc/self/exe");
    return str.substr(0, str.rfind('/'));
}


void Map::saveToFile(string filename)
{
    ofstream fout;
    fout.open(filename.c_str());
    if (fout.fail())
    {
        cout << "Error in saving records." << endl;
        return;
    }

    fout << "Score " << score << endl;

    // pacman
    fout << "startx " << pacman->start_x << endl;
    fout << "starty " << pacman->start_y << endl;
    fout << "lives " << pacman->lives << endl;
    fout << "eatenghosts " << pacman->eaten_ghosts << endl;

    // ghosts
    fout << "ghostnumber " << ghosts->size() << endl;
    for (int i=0; i<ghosts->size(); ++i)
    {
        fout << "startx " << (*ghosts)[i].start_x << endl;
        fout << "starty " << (*ghosts)[i].start_y << endl;
    }

    // map
    for (size_t i=0; i<vals.size(); ++i)
        fout << vals[i] << endl;

    fout.close();
    return;
}

int read_line(string line)
{
    istringstream record(line);
    string name;
    int value;
    record >> name;
    record >> value;
    return value;
}

void Map::readFromFile(string filename, PacMan &_pacman, vector<Ghost> &_ghosts)
{
    ifstream fin;
    fin.open(filename.c_str());
    if (fin.fail())
    {
        cout << "Error in opening file " << filename << endl;
        return;
    }

    string line;

    getline(fin,line);
    score = read_line(line);

    // _pacman
    int pacman_start_x, pacman_start_y;
    getline(fin,line);
    pacman_start_x = read_line(line);
    getline(fin,line);
    pacman_start_y = read_line(line);
    _pacman = PacMan(pacman_start_x, pacman_start_y);

    getline(fin,line);
    _pacman.lives = read_line(line);
    getline(fin,line);
    _pacman.eaten_ghosts = read_line(line);

    // _ghosts
    getline(fin,line);
    int ghost_number = read_line(line);
    for (int i=0; i<ghost_number; ++i)
    {
        int ghost_start_x, ghost_start_y;
        getline(fin,line);
        ghost_start_x = read_line(line);
        getline(fin,line);
        ghost_start_y = read_line(line);
        _ghosts.emplace_back(Ghost(ghost_start_x, ghost_start_y));
    }

    // vals
    while (getline(fin,line))
        vals.emplace_back(line);

    fin.close();

    // read vals
    int pacman_count = 0, ghost_count = 0;
    for (size_t i = 0; i < vals.size(); i++)
        for (size_t j = 0; j < vals[i].length(); j++)
        {
            if (vals[i][j] == 'o')
            {
                pacman_count++;
                if (pacman_count != 1)
                {
                    cout << "ERROR: Map includes more than 1 pacman." << endl;
                    return;
                }
                _pacman.x = i;
                _pacman.y = j;
                pacman = &_pacman;
                vals[i][j] = ' ';
            }
            if (vals[i][j] == 'E' || vals[i][j] == 'e')
            {
                _ghosts[ghost_count].x = i;
                _ghosts[ghost_count].y = j;
                if (vals[i][j] == 'E')
                    _ghosts[ghost_count].in_counteratk_mode = false;
                else
                    _ghosts[ghost_count].in_counteratk_mode = true;
                ghost_count++;
                vals[i][j] = ' ';
            }
        }
    ghosts = &_ghosts;

    if (pacman_count < 1)
        cout << "ERROR: Map does not include pacman." << endl;

    return;
}
