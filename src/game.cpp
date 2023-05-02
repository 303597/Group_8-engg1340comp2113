#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <cursesw.h>
#include <filesystem>
#include "tools.h"
#include "characters.h"
#include "game.h"
#include "ui.h"
#include <fstream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>

using namespace std;

int score = 0; // initialize score to be zero

//int total_num;


void saveScoreRecord(int level) // save high score
{
    bool need_update = false;
    string username;
    vector<ScoreRecord> histories = getScoreRecords();
    const char *res = getenv("USER");
    if (res == NULL)
        username = "Anonymous";
    else
        username = res;

    histories.emplace_back(ScoreRecord{username, score, level});
    sort(histories.rbegin(), histories.rend());
	// update the score record
    ofstream fout(getExecutablePath() + "/../data/score_record.txt");
    for (int i = 0; i < min((size_t)10, histories.size()); i++)
        fout << histories[i].username << " " << histories[i].score << " " << histories[i].level << endl;
    fout.close();

    score = 0; // reset score to zero after saving it
}

bool check(int x,int y, vector<Ghost*> ghosts){
    for (Ghost* ghost: ghosts)
    {
        if(x == ghost->start_x && y == ghost->start_y){
            return true;
        }
    }
    return false;
}

void generate_prop(Map* game_map, vector<Ghost*> ghosts, int &prop_lasting_time, int &prop_pos_x, int &prop_pos_y, int fruit_lasting_time)
{
	char prop[6] = {'@', '$', '*', '^', '!', '?'};
    int num;
    if(fruit_lasting_time == 0)
    {
        num = rand() % 6; // generate random number from [0,5] 
    }
    else
    {
        num = rand() % 5; // exclude '?'
    }
    int x = -1;
    int y = -1;
	while(1)
	{
        if(!(x == -1 && y == -1) && game_map->vals[x][y] == ' ' && !check(x, y, ghosts))
        {
            break;
        }
		x = rand() % game_map->vals.size();
		y = rand() % game_map->vals[x].size(); // generate postion
	}
	game_map->vals[x][y] = prop[num];
	prop_lasting_time = 20 * ghosts.size();
    prop_pos_x = x;
    prop_pos_y = y;
    //mvprintw(35, 10, "%d %d %d", x, y, num);
    return;
}

void initializeGame(string filename)
{
    bool from_saved_data = true;
    if (filename == "")
            from_saved_data = false;
    for (int level = 1; level <= 4; level++)
    {
        nodelay(stdscr, true); // don't wait until input

        Game game = from_saved_data ? Game(filename) : Game(level);
        if (from_saved_data)
            level = game.level;
        /*
        // Map game_map = Map(filename, pacman, ghosts, from_saved_data);
            
            
            Map game_map = Map("/2_Monsters/map2.txt", pacman, ghosts);
            

                //randomly choose a map to open
                int x = 2 + rand() % (3);
            */

            // randomly choose a map to open in a particular level
            
        int game_result = game.startGame();
        // if life == 0, game ends
        if (from_saved_data && (game_result == 0 || game_result == 1))
            clearSavedData(filename);
        if (game_result == 1 || game_result == 2)
            break;
        from_saved_data = false;
    }
}

/*
Game::Game(int level)
{
    string path, filename;
    int y = 1 + rand() % (3); 
    string map_no = to_string(y); 
    if (level <= 3)
        filename = to_string(level + 1) + "_Monsters/map" + map_no + ".txt";
    else
        filename = "bonus/map.txt";
    path = getExecutablePath() + "/../map/" + filename;
    
    ifstream fin(path);
    if (fin.fail())
    {
        cout << "Failed to open file: " << filename << endl;
        return;
    }
    
    string str;
    vector<string> vals;
    while (getline(fin, str))
        game_map->vals.emplace_back(str);
        
    int pacman_count = 0, ghost_count = 0;
    for (size_t i = 0; i < game_map->vals.size(); i++)
        for (size_t j = 0; j < game_map->vals[i].length(); j++)
        {
            if (vals[i][j] == 'o')
            {
                pacman_count++;
                if (pacman_count != 1)
                {
                    cout << "ERROR: Map includes more than 1 pac-man." << endl;
                    return;
                }
                *pacman = PacMan(i, j); // to be changed
                vals[i][j] = ' ';
            }
            if (vals[i][j] == 'E')
            {
                ghost_count++;
		        ghosts.emplace_back(new Ghost(i, j)); // to be changed
                vals[i][j] = ' ';
                //ghosts.emplace_back(&_ghosts.back());
                //ghosts.emplace_back(new Ghost(i, j, this));
            }
        }
    game_map = new Map(vals);

    if (pacman_count < 1)
        cout << "ERROR: Map does not include pac-man starting point." << endl;
}
*/

Game::Game(string filename)
{
    string path;
    path = getExecutablePath() + "/../data/" + filename;

    ifstream fin(path);
    if (fin.fail())
    {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;

    getline(fin,line);
    level = read_line<int>(line);
    getline(fin,line);
    score = read_line<int>(line);
    getline(fin,line);
    in_counteratk_mode = read_line<bool>(line);
    getline(fin,line);
    turns = read_line<int>(line);
    getline(fin,line);
    ghost_speed = read_line<int>(line);

    
    // _pacman
    getline(fin,line);
    int pacman_start_x = read_line<int>(line);
    getline(fin,line);
    int pacman_start_y = read_line<int>(line);
    
    pacman = new PacMan(pacman_start_x, pacman_start_y);
    
    getline(fin,line);
    pacman->x = read_line<int>(line);
    getline(fin,line);
    pacman->y = read_line<int>(line);
    getline(fin,line);
    pacman->lives = read_line<int>(line);
    getline(fin,line);
    pacman->eaten_ghosts = read_line<int>(line);

    // _ghosts
    getline(fin,line);
    int ghost_number = read_line<int>(line);
    for (int i=0; i<ghost_number; ++i)
    {
        getline(fin,line);
        int ghost_start_x = read_line<int>(line);
        getline(fin,line);
        int ghost_start_y = read_line<int>(line);
        
        ghosts.emplace_back(new Ghost(ghost_start_x,ghost_start_y));
        
        getline(fin,line);
        ghosts.back()->x = read_line<int>(line);
        getline(fin,line);
        ghosts.back()->y = read_line<int>(line);
        getline(fin,line);
        ghosts.back()->speed = read_line<double>(line);
        getline(fin,line);
        ghosts.back()->in_counteratk_mode = read_line<bool>(line);
    }
    
    // tools
    getline(fin,line);
    prop_type = read_line<int>(line);
    getline(fin,line);
    prop_turns = read_line<int>(line);
    getline(fin,line);
    prop_lasting_time = read_line<int>(line);
    getline(fin,line);
    prop_pos_x = read_line<int>(line);
    getline(fin,line);
    prop_pos_y = read_line<int>(line);
    
    // fruit
    getline(fin,line);
    fruit_num = read_line<int>(line);
    getline(fin,line);
    fruit_lasting_time = read_line<int>(line);
    getline(fin,line);
    fruit_pos_x = read_line<int>(line);
    getline(fin,line);
    fruit_pos_y = read_line<int>(line);
    
    // vals
    vector<string> vals;
    while (getline(fin,line))
        vals.emplace_back(line);
    game_map = new Map(vals); // Notice
}


int Game::pauseGame()
{
    nodelay(stdscr, false);
    saveToFile("temp.txt");
    while (true)
    {
        int operation = pauseLoop();

        if (operation == 1)
        {
            nodelay(stdscr, true);
            return 0;
        }

        if (operation == 2)
            showTutorial();

        if (operation == 3)
            showHighScore();

        if (operation == 4)
        {
            nodelay(stdscr, true);
            return 1;
        }
    }
}

void Game::saveToFile(string filename)
{
    ofstream fout;
    fout.open(getExecutablePath() + "/../data/" + filename);
    if (fout.fail())
    {
        cout << "Error in saving records." << endl;
        return;
    }

    fout << "level " << level << endl;
    fout << "score " << score << endl;
    fout << "in_counteratk " << in_counteratk_mode << endl;
    fout << "turns " << turns << endl;
    fout << "ghost_speed " << ghost_speed << endl;
    
    // pacman
    fout << "pacman_startx " << pacman->start_x << endl;
    fout << "pacman_starty " << pacman->start_y << endl;
    fout << "pacman_x " << pacman->x << endl;
    fout << "pacman_y " << pacman->y << endl;
    fout << "pacman_lives " << pacman->lives << endl;
    fout << "pacman_eatenghosts " << pacman->eaten_ghosts << endl;

    // ghosts
    fout << "ghostnumber " << ghosts.size() << endl;
    for (int i=0; i<ghosts.size(); ++i)
    {
        fout << "ghost_startx " << ghosts[i]->start_x << endl;
        fout << "ghost_starty " << ghosts[i]->start_y << endl;
        fout << "ghost_x " << ghosts[i]->x << endl;
        fout << "ghost_y " << ghosts[i]->y << endl;
        fout << "ghost_speed " << ghosts[i]->speed << endl;
        fout << "in_counteratk " << ghosts[i]->in_counteratk_mode << endl;
    }

    // tools
    fout << "prop_type " << prop_type << endl;
    fout << "prop_turns " << prop_turns << endl;
    fout << "prop_lasting_time " << prop_lasting_time << endl;
    fout << "prop_x " << prop_pos_x << endl;
    fout << "prop_y " << prop_pos_y << endl;
    
    
    // fruit
    fout << "fruit_num " << fruit_num << endl;
    fout << "fruit_lasting_time " << fruit_lasting_time << endl;
    fout << "fruit_x " << fruit_pos_x << endl;
    fout << "fruit_y " << fruit_pos_y << endl; 

    // map
    for (size_t i=0; i < game_map->vals.size(); ++i)
        fout << game_map->vals[i] << endl;

    fout.close();
}

void Game::showStatus()
{
    game_menu->showInGame(score, pacman->lives);
    game_map->show();
    pacman->show();
    for (Ghost* ghost: ghosts)
        ghost->show();
    refresh();
}

//check whether a map is completed
/*
bool isMapCompleted(const string& filename) {
    ifstream file(filename);
    if (!file) {
        // cerr << "Error: could not open file " << filename << endl;
        return false;
    }
    char ch = '.';
    char c;
    while (file >> noskipws >> c) {
        if (c == ch) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
*/

Game::Game(int _level)
{
    level = _level;

    string map_no = to_string(1 + rand() % 3), filename;
    if (level <= 3)
        filename = "/" + to_string(level + 1) + "_Monsters/map" + map_no + ".txt";
    else
        filename = "/bonus/map.txt";
    ifstream fin(getExecutablePath() + "/../map/" + filename);
    if (fin.fail())
    {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string str;
    vector<string> vals;
    while (getline(fin, str))
        vals.emplace_back(str);

    int pacman_count = 0, ghost_count = 0;
    //cookie_count = 0; // add this to game.h, set as public
    for (size_t i = 0; i < vals.size(); i++)
    {
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
                pacman = new PacMan(i, j);
                vals[i][j] = ' ';
            }
            if (vals[i][j] == 'E')
            {
                ghost_count++;
                ghosts.emplace_back(new Ghost(i, j));
                vals[i][j] = ' ';
            }
	    // to be implemented
        }
    }
    game_map = new Map(vals);

    prop_pos_x = game_map->vals.size() - 1; prop_pos_y = game_map->vals[prop_pos_x].size() - 1;
    fruit_pos_x = game_map->vals.size() - 1; fruit_pos_y = game_map->vals[fruit_pos_x].size() - 1;

    if (pacman_count < 1)
        cout << "ERROR: Map does not include pac-man starting point." << endl;
}

Game::~Game()
{
    delete game_map;
    delete pacman;
    for (int i = 0; i < ghosts.size(); i++)
        delete ghosts[i];
    delete game_menu;
}

void Game::checkCharacterCollision()
{
    for (int i = 0; i < ghosts.size(); ++i)
	{
		if (ghosts[i]->x == pacman->x && ghosts[i]->y == pacman->y)
		{
			// determine the mode of the ghost
			if (ghosts[i]->in_counteratk_mode)
			{
				pacman->eaten_ghosts++;
				if(special == "double_points")
				{
					score += 100 * pacman->eaten_ghosts;
				}
				else
				{
					score += 50 * pacman->eaten_ghosts; // modify the extern score variable directly
				}
				ghosts[i]->x = ghosts[i]->start_x;
				ghosts[i]->y = ghosts[i]->start_y;
				ghosts[i]->in_counteratk_mode = false;
				ghosts[i]->speed -= 5;
				// the eaten ghost reset to normal mode
			}
			else
			{
				pacman->lives--;
				direction = -2;
				if (pacman->lives == 0)
				{
					//mvprintw(12, 12, "%d", score);
					refresh();
					nodelay(stdscr, false);
					mvprintw(13, 12, "Press any key to quit.");
					getch();
					return; // player dies
				}
				pacman->x = pacman->start_x;
				pacman->y = pacman->start_y;
				for (int j = 0; j < ghosts.size(); ++j)
				{
					ghosts[j]->x = ghosts[j]->start_x;
					ghosts[j]->y = ghosts[j]->start_y;
					ghosts[j]->in_counteratk_mode = false;
					ghosts[j]->speed -= 5;
					turns = 0;
				}
				fruit_lasting_time = 0;
				prop_lasting_time = 10 * ghosts.size();
				prop_turns = 0;
				special = "none";
				// player and all the ghosts reset
			}
		}
	}
	int fl = 0;
	for(int i = 0; i < ghosts.size(); i++)
	{
		if(ghosts[i]->in_counteratk_mode == true)
		{
			fl = 1;
		}
	}
	if(fl == 0)
	{
		turns = 0;
	}
	// note: check counteratk mode, handle eat results
}

int Game::startGame()
{
    int dirx[4] = {-1, 1, 0, 0};//up down left right
    int diry[4] = {0, 0, -1, 1};

    /*
    prop_turns = 0, prop_lasting_time = 0, prop_type = -1, fruit_lasting_time = 0;
    prop_pos_x = game_map->vals.size() - 1;
    prop_pos_y = game_map->vals[prop_pos_x].size() - 1;
    fruit_pos_x = game_map->vals.size() - 1;
    fruit_pos_y = game_map->vals[fruit_pos_x].size() - 1;
    */

    char fruits[4] = {'1', '2', '3', '4'};
   
    game_menu = new Menu("in_game.txt");
    game_map->vals[0][0] = '#';
    pacman->linkMap(game_map);
    for (Ghost* ghost: ghosts)
        ghost->linkMap(game_map);
    
    showStatus();
    int ghost_rand = rand() % ghosts.size();
    int fruit_x = game_map->vals.size() - 1;
    int fruit_y = game_map->vals[fruit_x].size() - 1;

    auto last_frame_time = chrono::high_resolution_clock::now(), this_frame_time = last_frame_time;

    while (true)
    {
        int operation = 0;
        while (chrono::duration_cast<chrono::milliseconds>(this_frame_time - last_frame_time).count() < time_per_loop)
        {
            int ch = getch();
            if (ch != ERR)
                operation = ch;
                switch (operation)
                {
                    case 'w':
                    case KEY_UP:
                        direction = 0;
                        break;
                    case 's':
                    case KEY_DOWN:
                        direction = 1;
                        break;
                    case 'a':
                    case KEY_LEFT:
                        direction = 2;
                        break;
                    case 'd':
                    case KEY_RIGHT:
                        direction = 3;
                        break;
                    case 'p':
                        int result = pauseGame();
                        if (result == 1)
                            return 2;
                        break;
                }
                if(direction == -2)
                    continue;
            this_frame_time = chrono::high_resolution_clock::now();
        }
        switch (operation)
        {
            case 'w':
            case KEY_UP:
                direction = 0;
                break;
            case 's':
            case KEY_DOWN:
                direction = 1;
                break;
            case 'a':
            case KEY_LEFT:
                direction = 2;
                break;
            case 'd':
            case KEY_RIGHT:
                direction = 3;
                break;
            case 'p':
                int result = pauseGame();
                if (result == 1)
                    return 2;
                break;
        }
        pacman->move(direction, special);
        
        if (score >= ghosts.size()*150 && prop_lasting_time == 0)
        {
            generate_prop(game_map, ghosts, prop_lasting_time, prop_pos_x, prop_pos_y, fruit_lasting_time);
        }
        if(score >= game_map->total_num * 5 / 3 + 50 * ghosts.size() / 3)
        {
            ghost_speed = 0.8;
        }
        if(score >= game_map->total_num * 10 / 3 + 100 * ghosts.size() / 3)
        {
            ghost_speed = 1.0;
        }
        int tile_info = game_map->updateTile(pacman->x, pacman->y, ghosts, special, prop_turns);
        if (tile_info == 8)
        {
	        // if the pacman eats another super bean before the effect of the former super bean ends
	        if (turns > 0)
		    {
		        pacman->eaten_ghosts = 0;
	    	}
	        turns = ghosts.size() * 15; 
	        // reset count time and number of eaten ghosts to zero and count from start again
            for (Ghost* ghost: ghosts)
            {
                ghost->in_counteratk_mode = true;
            }
        }
        else if(tile_info == 9)
        {
            fruit_lasting_time = 0;
        }
        else if(tile_info <= 5 && tile_info >= 0)
        {
            prop_type = tile_info;
            if(tile_info == 5)
            {
                fruit_num = rand() % 10;
                if(fruit_num <= 3){fruit_num = 0;}
                else if(fruit_num <= 6 && fruit_num >= 4){fruit_num = 1;}
                else if(fruit_num <= 8 && fruit_num >=7){fruit_num = 2;}
                else{fruit_num = 3;}
                switch(fruit_num)
                {
                    case 0:
                    case 1:
                        while(1)
                        {
                            if(!(fruit_x == 0 && fruit_y == 0) && game_map->vals[fruit_x][fruit_y] == ' ' && !check(fruit_x, fruit_y, ghosts))
                            {
                                break;
                            }
                            fruit_x = rand() % game_map->vals.size();
                            fruit_y = rand() % game_map->vals[fruit_x].size();
                        }
                        fruit_lasting_time = 10 * ghosts.size() + fruit_num*5;
                        break;
                    case 2:
                        for(int i = 0; i < 4; i++)
                        {
                            if(game_map->vals[ghosts[ghost_rand]->x + dirx[i]][ghosts[ghost_rand]->y + diry[i]] != '#')
                            {
                                fruit_x = ghosts[ghost_rand]->x + dirx[i];
                                fruit_y = ghosts[ghost_rand]->y + diry[i];
                            }
                        }
                        fruit_lasting_time = 10 * ghosts.size();
                        break;
                    case 3:
                        fruit_x = ghosts[ghost_rand]->start_x;
                        fruit_y = ghosts[ghost_rand]->start_y;
                        fruit_lasting_time = 8 * ghosts.size();
                        break;
                    }
                    fruit_pos_y = fruit_y;
                    game_map->vals[fruit_x][fruit_y] = fruits[fruit_num];
                    fruit_x = game_map->vals.size() - 1;
                    fruit_y = game_map->vals[fruit_x].size() - 1;
                }
                prop_lasting_time = 10 * ghosts.size(); 
                tile_info = 6;
        }

        if(special == "magnet")
        {
            int xx, yy;
            for(int a = -1; a <= 1; a++)
            {
                for(int b = -1; b <= 1; b++)
                {
                    xx = pacman->x + a; yy = pacman->y + b;
                    if (xx < 0)
                    {
                        xx += game_map->vals.size();
                    }
                    else if (xx > game_map->vals.size() - 1)
                    {
                        xx -= game_map->vals.size();
                    }
                    if (yy < 0)
                    {
                        yy += game_map->vals[pacman->x].size();
                    }
                    else if (yy > game_map->vals[pacman->x].size() - 1)
                    {
                        yy -= game_map->vals[pacman->x].size();
                    }
                    // deal with overflow;
                    if(game_map->vals[xx][yy] == '.')
                    {
                        score += 5;
                        game_map->vals[xx][yy] = ' ';
                    }
                }
            }
        }

        for (Ghost* ghost: ghosts)
        {
            if(ghost->in_counteratk_mode == true)
            {
                ghost_speed = 0.6;
            }
            if(special == "slow")
            {
                ghost->speed += ghost_speed * 0.5;
            }
            else if(special == "frozen")
            {
                ghost->speed += 0;
            }
            else
            {
                ghost->speed += ghost_speed;
            }
        }
        if(turns == 0){
	        pacman->eaten_ghosts = 0; // reset the number of eaten ghosts to zero for the next round
            for (Ghost* ghost: ghosts)
            {
                ghost->in_counteratk_mode = false;
            }
        }
        if(turns != 0){
            turns--;
        }
        if(prop_lasting_time != 0)
        {
            prop_lasting_time--;
        }
        if(fruit_lasting_time != 0)
        {
            fruit_lasting_time--;
        }
        if(prop_turns != 0)
        {
            prop_turns--;
        }
        
        checkCharacterCollision();

        for (Ghost* ghost: ghosts)
        {
            if(ghost->speed >= 1.0)
            {
                ghost->speed -= 1.0;
                ghost->move(pacman->x, pacman->y, ghost_speed);
            }
        }

        checkCharacterCollision();

        if(prop_lasting_time == 10 * ghosts.size())
        {
            game_map->vals[prop_pos_x][prop_pos_y] = ' ';
        }
        if(fruit_lasting_time == 0)
        {
            game_map->vals[fruit_pos_x][fruit_pos_y] = ' ';
        }
        if(prop_turns == 0)
        {
            special = "none";
        }

        showStatus();
        mvprintw(23, 104, "%d" ,turns);
        if(prop_lasting_time >= 10 * ghosts.size())
        {
            mvprintw(24, 104, "%ld", prop_lasting_time - 10 * ghosts.size());
        }
        else if(prop_turns != 0)
        {
            mvprintw(24, 104, "%d", prop_turns);
        }
        else
        {
            mvprintw(24, 104, "%d", 0);
        }
        mvprintw(25, 104, "%d", fruit_lasting_time);

        refresh();
        last_frame_time = this_frame_time;
        mvprintw(0, 0, "🟦"); // move cursor
        if (pacman->lives <= 0)
        {
            saveScoreRecord(level);
            return 1;
        }
        //mvprintw(35, 10, "Total num: %d", game_map->total_num);
	if (game_map->total_num == 0)
        {
            return 0;
        }
	/*
	else
        {
            
            bool result = isMapCompleted(filename);
            if (result)
            {
                return true;
            }
            
        }
	*/

    }
}
