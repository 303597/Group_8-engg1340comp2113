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

// This function decides the cookie number by reading the map vector
Map::Map(vector<string> _vals)
{
    vals = _vals;
    cookie_num = 0;
    for (int i = 0; i < vals.size(); i++)
        for (int j = 0; j < vals[i].size(); j++)
            if (vals[i][j] == '.' || vals[i][j] == '0')
                {
                    cookie_num++;
                }
    mvprintw(5, 104, "%d", cookie_num);
    mvprintw(6, 104, "DDDDDD");
    refresh();
}

void Map::show()
{
    for (size_t i = 0; i < vals.size(); i++)
        for (size_t j = 0; j < vals[i].length(); j++)
            printElement(i, j, vals[i][j]);
}

// This function outprints the corresponding emojis
void printElement(int x, int y, char element)
{
    if (ascii)
    {
        switch (element)
    	{
		case 'o':
		    mvprintw(x + 2, 2 * y + 4, "OO");
		    break;
		case '#':
		    mvprintw(x + 2, 2 * y + 4, "##");
		    break;
		case '_':
		    mvprintw(x + 2, 2 * y + 4, "//");
		    break;
		case '.':
		    mvprintw(x + 2, 2 * y + 4, "··");
		    /*
		    if(!fl)
		    {
			cookie_num++;
		    }
		    */
		    break;
		case 'E':
		    mvprintw(x + 2, 2 * y + 4, "&&");
		    break;
		case 'e':
		    mvprintw(x + 2, 2 * y + 4, "&*");
		    break;
		case '0':
		    mvprintw(x + 2, 2 * y + 4, "oo");
		    break;
		case '@':
		    mvprintw(x + 2, 2 * y + 4, "UU");
		    break;
		case '$':
		    mvprintw(x + 2, 2 * y + 4, "$$");
		    break;
		case '*':
		    mvprintw(x + 2, 2 * y + 4, "~~");
		    break;
		case '^':
		    mvprintw(x + 2, 2 * y + 4, "vv");
		    break;
		case '!':
		    mvprintw(x + 2, 2 * y + 4, "》》");
		    break;
		case '?':
		    mvprintw(x + 2, 2 * y + 4, "??");
		    break;
		case '1':
		    mvprintw(x + 2, 2 * y + 4, "FF");
		    break;
		case '2':
		    mvprintw(x + 2, 2 * y + 4, "FF");
		    break;
		case '3':
		    mvprintw(x + 2, 2 * y + 4, "FF");
		    break;
		case '4':
		    mvprintw(x + 2, 2 * y + 4, "FF");
		    break;
		default:
		    mvprintw(x + 2, 2 * y + 4, "  ");
		    break;
	}
	return;
    }
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
                cookie_num++;
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

// Check what a tile contains and update the map accordingly
int Map::updateTile(int x, int y, vector<Ghost*> ghosts, string &special, int &prop_turns)
{
    int num;
    switch(vals[x][y])
    {
    	case '0':
	        num = 8;
            cookie_num--;
	        break;
    	case '.':
            if(special == "double_points")
            {
                score += 10;
            }
            else
            {
                score += 5;
            }
            cookie_num--;
		//cookie_count--;
	        num = 7;
	        break;
        case '@':
            special = "magnet";
            prop_turns = 7 * ghosts.size();
            num = 0;
            break;
        case '$':
            special = "double_points";
            prop_turns = 7 * ghosts.size();
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
            prop_turns = 10 * ghosts.size();
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

// This function to get the path of the execuatable is found at https://stackoverflow.com/questions/1528298/get-path-of-executable
string getExecutablePath() 
{
    string str = filesystem::canonical("/proc/self/exe");
    return str.substr(0, str.rfind('/'));
}

// Delete the saved file
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

// Show a menu similar to the welcome menu
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

// Show a menu similar to the in-game menu
void Menu::showInGame(int score, int lives)
{
    int line_no = 0;
    attron(COLOR_PAIR(1));
    for (string line: vals)
    {
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
    mvprintw(20, 98, "%5d", score);
    mvprintw(21, 98, "%5d", lives);
    //mvprintw(14, 100, "%5d", turns);
}

// This function reads from score record file
vector<ScoreRecord> getScoreRecords()
{
    ifstream fin(getExecutablePath() + "/../data/score_record.txt");
    vector<ScoreRecord> histories;
    
    if (!fin.fail())
    {
        string history_username;
        int history_score = 0, level_obtained = 0;
        while (fin >> history_username >> history_score >> level_obtained)
            histories.emplace_back(ScoreRecord{history_username, history_score, level_obtained});
		//fin >> history_username >> history_score;
	}
    fin.close();
    return histories;
}

string& Map::operator[](int index)
{
    return vals[index];
}

pair<size_t, size_t> Map::size()
{
    return make_pair(vals.size(), vals[0].size());
}
