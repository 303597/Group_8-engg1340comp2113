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

using namespace std;

int score = 0; // initialize score to be zero

//void save() //high score or map
void save()
{
	bool need_update = false;
	ifstream fin(getExecutablePath() + "/../data/score_record.txt", ios::in | ios::out);
    int history_score = 0;
	if (fin.fail())
	{
		//cout << "Error in opening the file." << endl;
        need_update = true;
	}
	else
	{
		fin >> history_score;
		if (score > history_score)
			need_update = true;
	}
    fin.close();
	// update the score record if the score this round is higher than history highest score
	if (need_update)
    {
        ofstream fout(getExecutablePath() + "/../data/score_record.txt");
        fout << score << endl;
        fout.close();
    }        
	
	score = 0; // reset score to zero after saving it
}

/*int generate_prop(Map &game_map, int &prop_lasting_time)
{
	char prop[5] = {'@', '$', '*', '^', '!', '?'};
	int num = rand() % 6; // generate random number from [0,5]
	int x = 0, y = 0;// position
	while(game_map.vals[x][y] != ' ')
	{
		x = rand() % game_map.vals.size();
		y = rand() % game_map.vals[0].size(); // generate postion
	}
	game_map.vals[x][y] = prop[num];
	prop_lasting_time = 20;
	return x*10 + y; // return position
}*/

int welcomeLoop()
{
    cbreak();
    ifstream fin(getExecutablePath() + "/../ui/start_menu.txt");
    if (fin.fail())
        cout << "Error opening file." << endl;
    string line;
    int selected = 0, line_no = 0;
    bool confirmed = false;
    vector<string> start_menu;
    while (getline(fin, line))
        start_menu.emplace_back(line);
    fin.close();
    while (!confirmed)
    {
        line_no = 0;
        for (string line: start_menu)
        {
            if ((line_no >= 14) && (line_no - 14) / 3 == selected)
                attron(COLOR_PAIR(2));
            else
                attron(COLOR_PAIR(1));
            mvprintw(line_no, 0, "%s", line.c_str());
            line_no++;
        }
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
    //vector<string> game_menu;

    bool in_counteratk_mode = false;
    int turns = 0;
    //int prop_turns = 0, prop_lasting_time = 0, prop_type = -1, fruit_lating_time;
    //int prop_pos = 0, fruit_pos = 0;
    //int fruit_x = 0, fruit_y = 0, fruit_num;
    //char fruits[4] = {'1', '2', '3', '4'};
    //char special = 'none';
    double ghost_speed = 0.4;
    PacMan pacman; vector<Ghost> ghosts;
    
    string map_path = getExecutablePath() + "/../map";
    Map game_map = Map(map_path + "/2_Monsters/map2.txt", pacman, ghosts);
    pacman.linkMap(&game_map);
    for (size_t i = 0; i < ghosts.size(); i++)
        ghosts[i].linkMap(&game_map);
    game_map.show();

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
        pacman.move(direction);
        /*if (score >= ghosts.size() * 150 && prop_lasting_time == 0)
	{
	    generate_prop(gampe_map);
	}*/
	if(score >= game_map.total_num * 5 / 3 + 50 * ghosts.size() / 3)
	{
             ghost_speed = 0.8;
	}
	if(score >= game_map.total_num * 10 / 3 + 100 * ghosts.size() / 3)
	{
             ghost_speed = 1.0;
	}
        int tile_info = game_map.updateTile(pacman.x, pacman.y);
        if (tile_info == 1)//if (tile_info == 8)
        {
	        // if the pacman eats another super bean before the effect of the former super bean ends
	        if (turns > 0)
		{
		    pacman.eaten_ghosts = 0;
		}
	        turns = 1; 
	        // reset count time and number of eaten ghosts to zero and count from start again
            for (Ghost &ghost: ghosts)
            {
                ghost.in_counteratk_mode = true;
            }
        }
	/*
	else if(tile_info <= 5 && tile_info >= 0)
	{
	    prop_type = tile_info;
	    switch(tile_info)
	    {
	    	case 0:
			magnet = true;
			prop_turns = 10;
			break;
		case 1:
			double_points = true;
			prop_turns = 10;
			break;
		case 2:
			ghost_speed = 0;
			prop_turns = 6;
			break;
		case 3:
			ghost_speed *= 0.5;
			prop_turns = 10;
			break;
		case 4:
			pass_bricks = true;
			prop_turns = 15;
			break;
		case 5:
			fruit_num = rand() % 4;
			switch(fruit_num)
			{
				case 0:
					while(game_map.vals[fruit_x][fruit_y] != ' ')
					{
						fruit_x = rand() % game_map.vals.size();
						fruit_y = rand() % game_map.vals[fruit_x].size();
					}
					fruit_lastin_time = 25;
					break;
				case 1:
					while(game_map.vals[fruit_x][fruit_y] != ' ')
                                        {
                                                 fruit_x = rand() % game_map.vals.size();
                                                 fruit_y = rand() % game_map.vals[fruit_x].size();
                                        }
					fruit_lasting_time = 15;
					break;
				case 2:
					int ghost_rand = rand() % ghosts.size();
					int dirx[4] = {-1, 1, 0, 0}//up down left right
					int diry[4] = {0, 0, -1, 1}
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
                                       int ghost_rand = rand() % ghosts.size();
				       fruit_x = ghosts[ghost_rand].start_x;
                                       fruit_y = ghosts[ghost_rand].start_y;
				       fruit_lasting_time = 15;
				       break;
			}
			break;
		case 9:
			fruit_lasting_time = 15;
			break;
	    }
	    if(!(fruit_x == 0 && fruit_y == 0))
	    {
	    	fruit_pos = fruit_x*10 + fruit_y;
		game_map[fruit_x][fruit_y] = fruits[fruit_num];
		fruit_x = 0; fruit_y = 0;
	    }
	    prop_turns = 0;
	    prop_lasting_time = 5; 
	    tile_info = 6;
	}
	*/
        for (Ghost &ghost: ghosts)
        {
            if(ghost.in_counteratk_mode == true)
            {
	    	if(tile_info == 3)
		{
			ghost.speed += 0.3;
		}
                ghost.speed += 0.6;
            }
            else
            {
                ghost.speed += ghost_speed;
            }
        }
        if(turns == ghosts.size() * 15){
            turns = 0;
	    pacman.eaten_ghosts = 0; // reset the number of eaten ghosts to zero for the next round
            for (Ghost &ghost: ghosts)
            {
                ghost.in_counteratk_mode = false;
            }
        }
        if(turns != 0){
            turns++;
        }
        /*
	if(prop_lasting_time != 0)
	{
		prop_lasting_time--;
	}
	if(fruit_lasting_time != 0)
	{
		fruit_lasting_time--;
	}
	*/
        checkCharacterCollision(pacman, ghosts, turns, direction);

        for (Ghost &ghost: ghosts)
        {
            if(ghost.speed >= 1.0)
            {
                ghost.speed -= 1.0;
                ghost.move(pacman.x, pacman.y, ghost_speed);
            }
        }

        checkCharacterCollision(pacman, ghosts, turns, direction);

	/*if(prop_lasting_time == 0)
        {
                game_map[prop_pos/10][prop_pos%10] = ' ';
        }
	if(prop_lasting_time == 0)
        {
                 game_map[prop_pos/10][prop_pos%10] = ' ';
        }*/

	refresh();

        game_map.show();

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
    refresh();
    getch();
}
