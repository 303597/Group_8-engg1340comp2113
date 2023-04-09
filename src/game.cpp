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
        
        int tile_info = game_map.updateTile(pacman.x, pacman.y);
        if (tile_info == 1)
        {
            //turns++;
	        // if the pacman eats another super bean before the effect of the former super bean ends
	        if (turns > 0)      pacman.eaten_ghosts = 0;
	        turns = 1; 
	        // reset count time and number of eaten ghosts to zero and count from start again
            for (Ghost &ghost: ghosts)
            {
                ghost.in_counteratk_mode = true;
            }
        }
        if(score >= game_map.total_num * 5 / 3 + 50 * ghosts.size() / 3){
            ghost_speed = 0.8;
        }
        if(score >= game_map.total_num * 10 / 3 + 100 * ghosts.size() / 3){ 
            ghost_speed = 1.0;
        }
        for (Ghost &ghost: ghosts)
        {
            if(ghost.in_counteratk_mode == true)
            {
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