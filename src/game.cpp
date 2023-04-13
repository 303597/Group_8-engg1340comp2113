#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <ncurses.h>
#include <filesystem>
#include "tools.h"
#include "characters.h"
#include "game.h"
#include <fstream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>

using namespace std;

int score = 0; // initialize score to be zero

//void save() //high score or map
void save()
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
    //mvprintw(19, 12, "HHHHHHHH");
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
    //mvprintw(14, 12, "HHHHHHH");
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
	prop_lasting_time = 30;
    prop_pos_x = x;
    prop_pos_y = y;
    return;
}

int welcomeLoop()
{
    cbreak();
    
    int selected = 0, line_no = 0;
    bool confirmed = false;
    Menu start_menu("start_menu.txt");
    
    while (!confirmed)
    {
        start_menu.showWelcome(selected);
        refresh();
        int ch = getch();
        switch (ch)
        {
            case 'w':
            case KEY_UP:
                selected = max(0, selected - 1);
                break;
            case 's':
            case KEY_DOWN:
                selected = min(4, selected + 1);
                break;
            case ' ':
            case '\n':
                confirmed = true;
                break;
        }
    }
    return selected;
}

//void UI() //welcome, gamescore, end, user interfaces
bool gameLoop()
{
    nodelay(stdscr, true); // don't wait until input

    //int score = 0;
    Menu game_menu("in_game.txt");

    bool in_counteratk_mode = false;
    int turns = 0;
    double ghost_speed = 0.4;
    PacMan pacman; vector<Ghost> ghosts;
    int prop_turns = 0, prop_lasting_time = 0, prop_type = -1, fruit_lasting_time;
    int prop_pos_x = 0, prop_pos_y = 0, fruit_pos_x = 0, fruit_pos_y = 0;
    int fruit_num;
    char fruits[4] = {'1', '2', '3', '4'};
    string special = "none";
    int dirx[4] = {-1, 1, 0, 0};//up down left right
    int diry[4] = {0, 0, -1, 1};
    
    Map game_map = Map("/2_Monsters/map2.txt", pacman, ghosts);
    pacman.linkMap(&game_map);
   
    game_map.vals[0][0] = '#';
    for (size_t i = 0; i < ghosts.size(); i++)
        ghosts[i].linkMap(&game_map);
    game_menu.showInGame(score, pacman.lives);
    game_map.show();
    int ghost_rand = rand() % ghosts.size();
    int fruit_x = game_map.vals.size() - 1;
    int fruit_y = game_map.vals[fruit_x].size() - 1;

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
            if(direction == -2){
                continue;
            }
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
        }
        pacman.move(direction, special);
        if (score >= ghosts.size()*150 && prop_lasting_time == 0)
        {
            generate_prop(game_map, ghosts, prop_lasting_time, prop_pos_x, prop_pos_y, fruit_lasting_time);
        }
        if(score >= game_map.total_num * 5 / 3 + 50 * ghosts.size() / 3)
        {
            ghost_speed = 0.8;
        }
        if(score >= game_map.total_num * 10 / 3 + 100 * ghosts.size() / 3)
        {
            ghost_speed = 1.0;
        }
        int tile_info = game_map.updateTile(pacman.x, pacman.y, special);
        if (tile_info == 8)
        {
	        // if the pacman eats another super bean before the effect of the former super bean ends
	        if (turns > 0)
		    {
		        pacman.eaten_ghosts = 0;
	    	}
	        turns = ghosts.size() * 15; 
	        // reset count time and number of eaten ghosts to zero and count from start again
            for (Ghost &ghost: ghosts)
            {
                ghost.in_counteratk_mode = true;
            }
        }
        else if(tile_info <= 5 && tile_info >= 0)
        {
            prop_type = tile_info;
            switch(tile_info)
            {
                case 0:
                    special = "magnet";
                    prop_turns = 10;
                    break;
                case 1:
                    special = "double_points";
                    prop_turns = 10;
                    break;
                case 2:
                    special = "frozen";
                    prop_turns = 6;
                    break;
                case 3:
                    special = "slow";
                    prop_turns = 10;
                    break;
                case 4:
                    special = "pass_bricks";
                    prop_turns = 15;
                    break;
                case 5:
                    fruit_num = rand() % 10;
                    if(fruit_num <= 3){fruit_num = 0;}
                    else if(fruit_num <= 6 && fruit_num >= 4){fruit_num = 1;}
                    else if(fruit_num <= 8 && fruit_num >=7){fruit_num = 2;}
                    else{fruit_num = 3;}
                    mvprintw(17, 12, "%d", fruit_num);
                    switch(fruit_num)
                    {
                        case 0:
                            //mvprintw(18, 12, "HHHHHHHH");
                            while(1)
                            {
                                if(!(fruit_x == 0 && fruit_y == 0) && game_map.vals[fruit_x][fruit_y] == ' ' && !check(fruit_x, fruit_y, ghosts))
                                {
                                    break;
                                }
                                fruit_x = rand() % game_map.vals.size();
                                fruit_y = rand() % game_map.vals[fruit_x].size();
                            }
                            fruit_lasting_time = 25;
                            break;
                        case 1:
                            //mvprintw(18, 12, "HHHHHHHH");
                            while(1)
                            {
                                if(!(fruit_x == 0 && fruit_y == 0) && game_map.vals[fruit_x][fruit_y] == ' ' && !check(fruit_x, fruit_y, ghosts))
                                {
                                    break;
                                }
                                fruit_x = rand() % game_map.vals.size();
                                fruit_y = rand() % game_map.vals[fruit_x].size();
                            }
                            fruit_lasting_time = 15;
                            break;
                        case 2:
                            for(int i = 0; i < 4; i++)
                            {
                                if(game_map.vals[ghosts[ghost_rand].x + dirx[i]][ghosts[ghost_rand].y + diry[i]] != '#')
                                {
                                    fruit_x = ghosts[ghost_rand].x + dirx[i];
                                    fruit_y = ghosts[ghost_rand].y + diry[i];
                                }
                            }
                            fruit_lasting_time = 20;
                            break;
                        case 3:
                            fruit_x = ghosts[ghost_rand].start_x;
                            fruit_y = ghosts[ghost_rand].start_y;
                            fruit_lasting_time = 15;
                            break;
                    }
                    //mvprintw(14, 12, "%d", fruit_x);
                    //mvprintw(15, 12, "%d", fruit_y);
                    if(!(fruit_x == 0 && fruit_y == 0))
                    {
                        fruit_pos_x = fruit_x;
                        fruit_pos_y = fruit_y;
                        game_map.vals[fruit_x][fruit_y] = fruits[fruit_num];
                        //mvprintw(16, 12, "%c", game_map.vals[fruit_x][fruit_y]);
                        fruit_x = game_map.vals.size() - 1;
                        fruit_y = game_map.vals[fruit_x].size() - 1;
                    }
                    break;
                case 9:
                    break;
                prop_turns = 0;
                prop_lasting_time = 10; 
                tile_info = 6;
            }
        }
        if(special == "magnet")
        {
            int xx, yy;
            for(int a = -1; a <= 1; a++)
            {
                for(int b = -1; b <= 1; b++)
                {
                    xx = pacman.x + a; yy = pacman.y + b;
                    if (xx < 0)
                    {
                        xx += game_map.vals.size();
                    }
                    else if (xx > game_map.vals.size() - 1)
                    {
                        xx -= game_map.vals.size();
                    }
                    if (yy < 0)
                    {
                        yy += game_map.vals[pacman.x].size();
                    }
                    else if (yy > game_map.vals[pacman.x].size() - 1)
                    {
                        yy -= game_map.vals[pacman.x].size();
                    }
                    // deal with overflow;
                    if(game_map.vals[xx][yy] == '.')
                    {
                        score += 5;
                        game_map.vals[xx][yy] = ' ';
                    }
                }
            }
        }
        for (Ghost &ghost: ghosts)
        {
            if(ghost.in_counteratk_mode == true)
            {
                if(special == "slow")
                {
                    ghost.speed += 0.3;
                }
                else if(special == "frozen")
                {
                    ghost.speed += 0;
                }
                else
                {
                    ghost.speed += 0.6;
                }
            }
            else
            {
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
        }
        if(turns == 0){
	        pacman.eaten_ghosts = 0; // reset the number of eaten ghosts to zero for the next round
            for (Ghost &ghost: ghosts)
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

        for (Ghost &ghost: ghosts)
        {
            if(ghost.speed >= 1.0)
            {
                ghost.speed -= 1.0;
                ghost.move(pacman.x, pacman.y, ghost_speed);
            }
        }

        checkCharacterCollision(pacman, ghosts, turns, direction, prop_lasting_time, fruit_lasting_time, prop_turns, special);

        if(prop_lasting_time == 0)
        {
            game_map.vals[prop_pos_x][prop_pos_y] = ' ';
        }
        if(fruit_lasting_time == 0)
        {
            game_map.vals[fruit_pos_x][fruit_pos_y] = ' ';
        }
        if(prop_turns == 0)
        {
            special = "none";
        }

        game_menu.showInGame(score, pacman.lives);
        game_map.show();
        mvprintw(23, 104, "%d" ,turns);
        if(prop_lasting_time != 0)
        {
            mvprintw(24, 104, "%d", prop_lasting_time);
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
        if (pacman.lives <= 0)
        {
            save();
                return false;
        }
    }
}

void showTutorial()
{
    cbreak();
    
    ifstream fin(getExecutablePath() + "/../ui/tutorial.txt");
    if (fin.fail())
        cout << "Error opening file." << endl;
    
    string line;
    int line_no = 0;
    while (getline(fin, line))
    {
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
    refresh();
    getch();
}

void showHighScore()
{
    cbreak();
    
    ifstream fin(getExecutablePath() + "/../ui/high_scores.txt");
    if (fin.fail())
        cout << "Error opening file." << endl;
    
    string line;
    int line_no = 0;
    while (getline(fin, line))
    {
        mvprintw(line_no, 0, "%s", line.c_str());
        line_no++;
    }
    line_no = 15;
    vector<pair<string, int>> histories = getScoreRecords();
    if (histories.size() == 0)
        mvprintw(line_no, 49, "No score record yet.");
    for (int i = 0; i < min((size_t)10, histories.size()); i++)
    {
        mvprintw(line_no, 19, "%d", i + 1);
        mvprintw(line_no, 34, "%s", histories[i].first.c_str());
        mvprintw(line_no, 59, "%5d", histories[i].second);
        mvprintw(line_no, 84, "1");
        line_no++;
    }
    refresh();
    getch();
}
