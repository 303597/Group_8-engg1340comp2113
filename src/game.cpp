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

void saveScoreRecord() // save high score
{
    bool need_update = false;
    string username;
    vector<pair<string, int>> histories = getScoreRecords();
    const char *res = getenv("USER");
    if (res == NULL)
        username = "Anonymous";
    else
        username = res;

    histories.emplace_back(make_pair(username, score));
    sort(histories.begin(), histories.end(), [](pair<string, int> x, pair<string, int> y) { return x.second > y.second; } );
	// update the score record
    ofstream fout(getExecutablePath() + "/../data/score_record.txt");
    for (int i = 0; i < min((size_t)10, histories.size()); i++)
        fout << histories[i].first << " " << histories[i].second << endl;
    fout.close();

    score = 0; // reset score to zero after saving it
}

bool check(int x,int y, vector<Ghost> &ghosts){
    for (Ghost &ghost: ghosts)
    {
        if(x == ghost.start_x && y == ghost.start_y){
            return true;
        }
    }
    return false;
}

void generate_prop(Map &game_map, vector<Ghost> &ghosts, int &prop_lasting_time, int &prop_pos_x, int &prop_pos_y, int fruit_lasting_time)
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
    int x = game_map.vals.size() - 1;
    int y = game_map.vals[x].size() - 1;
	while(1)
	{
        if(!(x == 0 && y == 0) && game_map.vals[x][y] == ' ' && !check(x, y, ghosts))
        {
            break;
        }
		x = rand() % game_map.vals.size();
		y = rand() % game_map.vals[x].size(); // generate postion
	}
	game_map.vals[x][y] = prop[num];
	prop_lasting_time = 20 * ghosts.size();
    prop_pos_x = x;
    prop_pos_y = y;
    return;
}

void initializeGame(string filename)
{
    for (int level = 1; level <= 4; level++)
    {
        nodelay(stdscr, true); // don't wait until input

        PacMan pacman; vector<Ghost> ghosts;
        bool from_saved_data = true;
        if (filename == "") from_saved_data = false;
        Game game = from_saved_data ? Game(filename) : Game(level);
        if (filename == "")
        {
            int y = 1 + rand() % (3); 
            string map_no = to_string(y); 
            if (level <= 3)
                filename = "/" + to_string(level + 1) + "_Monsters/map" + map_no + ".txt";
            else
                filename = "/bonus/map.txt";
            from_saved_data = false;
        }
        // Map game_map = Map(filename, pacman, ghosts, from_saved_data);
            
            /*
            Map game_map = Map("/2_Monsters/map2.txt", pacman, ghosts);
            */

            /*
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
    // TO BE IMPLEMENTED
}

void Game::showStatus()
{
    game_menu->showInGame(score, pacman->lives);
    game_map->show();
    pacman->show();
    for (Ghost &ghost: (*ghosts))
        ghost.show();
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

//void UI() //welcome, gamescore, end, user interfaces
int Game::startGame() // TO BE DELETED
{
    int dirx[4] = {-1, 1, 0, 0};//up down left right
    int diry[4] = {0, 0, -1, 1};

    int prop_turns = 0, prop_lasting_time = 0, prop_type = -1, fruit_lasting_time = 0;
    int prop_pos_x = game_map->vals.size() - 1;
    int prop_pos_y = game_map->vals[prop_pos_x].size() - 1;
    int fruit_pos_x = game_map->vals.size() - 1;
    int fruit_pos_y = game_map->vals[fruit_pos_x].size() - 1;
    int fruit_num;
    char fruits[4] = {'1', '2', '3', '4'};
    string special = "none";
   
    Menu game_menu("in_game.txt");
    game_map->vals[0][0] = '#';
    for (Ghost &ghost: *ghosts)
        ghost.linkMap(game_map);
    
    showStatus();
    int ghost_rand = rand() % (*ghosts).size();
    int fruit_x = game_map->vals.size() - 1;
    int fruit_y = game_map->vals[fruit_x].size() - 1;

    auto last_frame_time = chrono::high_resolution_clock::now(), this_frame_time = last_frame_time;

    int direction = -2;
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
        if (score >= ghosts->size()*150 && prop_lasting_time == 0)
        {
            generate_prop(*game_map, *ghosts, prop_lasting_time, prop_pos_x, prop_pos_y, fruit_lasting_time);
        }
        if(score >= game_map->total_num * 5 / 3 + 50 * ghosts->size() / 3)
        {
            ghost_speed = 0.8;
        }
        if(score >= game_map->total_num * 10 / 3 + 100 * ghosts->size() / 3)
        {
            ghost_speed = 1.0;
        }
        int tile_info = game_map->updateTile(pacman->x, pacman->y, *ghosts, special, prop_turns);
        if (tile_info == 8)
        {
	        // if the pacman eats another super bean before the effect of the former super bean ends
	        if (turns > 0)
		    {
		        pacman->eaten_ghosts = 0;
	    	}
	        turns = ghosts->size() * 15; 
	        // reset count time and number of eaten ghosts to zero and count from start again
            for (Ghost &ghost: *ghosts)
            {
                ghost.in_counteratk_mode = true;
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
                        fruit_lasting_time = 10 * ghosts->size() + fruit_num*5;
                        break;
                    case 2:
                        for(int i = 0; i < 4; i++)
                        {
                            if(game_map->vals[(*ghosts)[ghost_rand].x + dirx[i]][(*ghosts)[ghost_rand].y + diry[i]] != '#')
                            {
                                fruit_x = (*ghosts)[ghost_rand].x + dirx[i];
                                fruit_y = (*ghosts)[ghost_rand].y + diry[i];
                            }
                        }
                        fruit_lasting_time = 10 * ghosts->size();
                        break;
                    case 3:
                        fruit_x = (*ghosts)[ghost_rand].start_x;
                        fruit_y = (*ghosts)[ghost_rand].start_y;
                        fruit_lasting_time = 8 * ghosts->size();
                        break;
                    }
                    fruit_pos_y = fruit_y;
                    game_map->vals[fruit_x][fruit_y] = fruits[fruit_num];
                    fruit_x = game_map->vals.size() - 1;
                    fruit_y = game_map->vals[fruit_x].size() - 1;
                }
                prop_lasting_time = 10 * ghosts->size(); 
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

        for (Ghost &ghost: *ghosts)
        {
            if(ghost.in_counteratk_mode == true)
            {
                ghost_speed = 0.6;
            }
            if(special == "slow")
            {
                ghost.speed += ghost_speed * 0.5;
            }
            else if(special == "frozen")
            {
                ghost.speed += 0;
            }
            else
            {
                ghost.speed += ghost_speed;
            }
        }
        if(turns == 0){
	        pacman->eaten_ghosts = 0; // reset the number of eaten ghosts to zero for the next round
            for (Ghost &ghost: *ghosts)
            {
                ghost.in_counteratk_mode = false;
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
        
        checkCharacterCollision(pacman, ghosts, turns, direction, prop_lasting_time, fruit_lasting_time, prop_turns, special);

        for (Ghost &ghost: *ghosts)
        {
            if(ghost.speed >= 1.0)
            {
                ghost.speed -= 1.0;
                ghost.move(pacman->x, pacman->y, ghost_speed);
            }
        }

        checkCharacterCollision(pacman, ghosts, turns, direction, prop_lasting_time, fruit_lasting_time, prop_turns, special);

        if(prop_lasting_time == 10 * ghosts->size())
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
        if(prop_lasting_time >= 10 * ghosts->size())
        {
            mvprintw(24, 104, "%ld", prop_lasting_time - 10 * ghosts->size());
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
            saveScoreRecord();
            return 1;
        }
	else
        {
            /*
            bool result = isMapCompleted(filename);
            if (result)
            {
                return true;
            }
            */
        }
    }
}
