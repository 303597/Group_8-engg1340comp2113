#include "tools.h"
#include "characters.h"
#include <cursesw.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;

Map::Map(string filename, PacMan &_pacman, vector<Ghost> &_ghosts, bool from_saved_data) // _pacman: passed by reference  pacman: belongs to Map
{
    string path;
    if (from_saved_data)
        path = getExecutablePath() + "/../data/" + filename;
    else
        path = getExecutablePath() + "/../map/" + filename;
    ifstream fin(path);
    if (fin.fail())
    {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    if (from_saved_data)
    {
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
        _pacman.x = read_line(line); // Notice
        getline(fin,line);
        _pacman.y = read_line(line);

        getline(fin,line);
        _pacman.lives = read_line(line);
        getline(fin,line);
        _pacman.eaten_ghosts = read_line(line);

        pacman = &_pacman; // Notice

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
            getline(fin,line);
            _ghosts.back().x = read_line(line);
            getline(fin,line);
            _ghosts.back().y = read_line(line);
	    getline(fin,line);
	    _ghosts.back().speed = read_line(line);
        }
        ghosts = &_ghosts;

        // vals
        while (getline(fin,line))
            vals.emplace_back(line);
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
            printElement(i, j, vals[i][j]);
        }
    }
    
    fl = true;
}

void printElement(int x, int y, char element)
{
    switch (element)
    {
        case 'o':
            mvprintw(x + 2, 2 * y + 4, "🟡");
            break;
        case '#':
            mvprintw(x + 2, 2 * y + 4, "🟦");
            break;
        case '_':
            mvprintw(x + 2, 2 * y + 4, "🟩");
            break;
        case '.':
            mvprintw(x + 2, 2 * y + 4, "◽");
            /*
            if(!fl)
            {
                total_num++;
            }
            */
            break;
        case 'E':
            mvprintw(x + 2, 2 * y + 4, "👻");
            break;
        case 'e':
            mvprintw(x + 2, 2 * y + 4, "🥶");
            break;
        case '0':
            mvprintw(x + 2, 2 * y + 4, "🟢");
            break;
        case '@':
            mvprintw(x + 2, 2 * y + 4, "🧲");
            break;
        case '$':
            mvprintw(x + 2, 2 * y + 4, "💰");
            break;
        case '*':
            mvprintw(x + 2, 2 * y + 4, "🧊");
            break;
        case '^':
            mvprintw(x + 2, 2 * y + 4, "🛹");
            break;
        case '!':
            mvprintw(x + 2, 2 * y + 4, "📜");
            break;
        case '?':
            mvprintw(x + 2, 2 * y + 4, "❓");
            break;
        case '1':
            mvprintw(x + 2, 2 * y + 4, "🍒");
            break;
        case '2':
            mvprintw(x + 2, 2 * y + 4, "🍑");
            break;
        case '3':
            mvprintw(x + 2, 2 * y + 4, "🍉");
            break;
        case '4':
            mvprintw(x + 2, 2 * y + 4, "🍇");
            break;
	    default:
            mvprintw(x + 2, 2 * y + 4, "  ");
            break;
    }
}

int Map::updateTile(int x, int y, vector<Ghost> ghosts, string &special, int &prop_turns)
{
    int num;
    switch(vals[x][y])
    {
    	case '0':
	        num = 8;
	        break;
    	case '.':
            if(special == "double_points")
            {
                score += 10;
            }
            else
            {
                score += 5;
                total_num--;
            }
	        num = 7;
	        break;
        case '@':
            special = "magnet";
            prop_turns = 10 * ghosts.size();
            num = 0;
            break;
        case '$':
            special = "double_points";
            prop_turns = 10 * ghosts.size();
            num = 1;
            break;
        case '*':
            special = "frozen";
            prop_turns = 6 * ghosts.size();
            num = 2;
            break;
        case '^':
            special = "slow";
            prop_turns = 8 * ghosts.size();
            num = 3;
            break;
        case '!':
            special = "pass_bricks";
            prop_turns = 14 * ghosts.size();
            num = 4;
            break;
        case '?':
            num = 5;
            break;
        case '1':
            score += 50;
            num = 9;
            break;
        case '2':
            score += 75;
            num = 9;
            break;
        case '3':
            score += 100;
            num = 9;
            break;
        case '4':
            score += 150;
            num = 9;
            break;
        default:
            num = 6;
            break;
    }
    if(vals[x][y] == '#' || vals[x][y] == '_')
    {
        num = 6;
        return num;
    }
    vals[x][y] = ' ';
    return num;
    /*if (vals[x][y] == '0')
    {
        vals[x][y] = ' ';
        return 1;
    }
    else if(vals[x][y] == '.')
    {
        score += 5;
        vals[x][y] = ' ';
        return 0;
    }
    
    return 2;*/
}

string getExecutablePath() // This solution to get the path of the execuatable is found at https://stackoverflow.com/questions/1528298/get-path-of-executable
{
    string str = filesystem::canonical("/proc/self/exe");
    return str.substr(0, str.rfind('/'));
}

void Map::saveToFile(string filename)
{
    ofstream fout;
    fout.open(getExecutablePath() + "/../data/" + filename);
    if (fout.fail())
    {
        cout << "Error in saving records." << endl;
        return;
    }

    fout << "Score " << score << endl;

    // pacman
    fout << "startx " << pacman->start_x << endl;
    fout << "starty " << pacman->start_y << endl;
    fout << "x " << pacman->x << endl;
    fout << "y " << pacman->y << endl;
    fout << "lives " << pacman->lives << endl;
    fout << "eatenghosts " << pacman->eaten_ghosts << endl;

    // ghosts
    fout << "ghostnumber " << ghosts->size() << endl;
    for (int i=0; i<ghosts->size(); ++i)
    {
        fout << "startx " << (*ghosts)[i].start_x << endl;
        fout << "starty " << (*ghosts)[i].start_y << endl;
	fout << "x " << (*ghosts)[i].x << endl;
        fout << "y " << (*ghosts)[i].y << endl;
	fout << "speed " << (*ghosts)[i].speed << endl;
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

void clearSavedData(string filename)
{
    if (!filesystem::remove(getExecutablePath() + "/../data/" + filename))
        cout << "Error deleting save file" << endl;
}

Menu::Menu(string filename)
{
    ifstream fin(getExecutablePath() + "/../ui/" + filename);
    if (fin.fail())
        cout << "Error opening file." << endl;
    string line;
    while (getline(fin, line))
        vals.emplace_back(line);
    fin.close();
}

void Menu::showWelcome(int selected)
{
    int line_no = 0;
    for (string line: vals)
    {
        if ((line_no >= 14) && (line_no - 14) / 3 == selected)
            attron(COLOR_PAIR(2));
        else
            attron(COLOR_PAIR(1));
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
}

void Menu::showInGame(int score, int lives)
{
    int line_no = 0;
    attron(COLOR_PAIR(1));
    for (string line: vals)
    {
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
    mvprintw(19, 94, "%5d", score);
    mvprintw(20, 94, "%5d", lives);
    //mvprintw(14, 100, "%5d", turns);
}

vector<pair<string, int>> getScoreRecords()
{
    ifstream fin(getExecutablePath() + "/../data/score_record.txt");
    vector<pair<string, int>> histories;
    
    if (!fin.fail())
    {
        string history_username;
        int history_score = 0;
        while (fin >> history_username >> history_score)
            histories.emplace_back(make_pair(history_username, history_score));
		fin >> history_username >> history_score;
	}
    fin.close();
    return histories;
}
