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

//void UI() //welcome, gamescore, end, user interfaces

int ui_wide = 120;
int ui_height = 40;

int welcome_ui(int i, int j, int index_) {
    // 20 * 90
    int ii = i - 1 - 9;
    int jj = j - 1 - 14;
    int index = index_;

    string menu0[20] =
    {
    "—————————————————————————————————————————————",
    "|               Use W / S to move upwards / downwards, and use y to choose               |",
    "|                                                                                        |",
    "|                      ————————————————————————                  |",
    "|                      |-------------> Start New Game <---------------|                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                Game Tutorial                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |               History Records                |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |            Continue previous game            |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                     Exit                     |                  |",
    "|                      ————————————————————————                  |",
    "|                                                                                        |",
    "—————————————————————————————————————————————",
    };

    string menu1[20] =
    {
    "—————————————————————————————————————————————",
    "|               Use W / S to move upwards / downwards, and use y to choose               |",
    "|                                                                                        |",
    "|                      ————————————————————————                  |",
    "|                      |               Start New Game                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |--------------> Game Tutorial <---------------|                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |               History Records                |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |            Continue previous game            |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                     Exit                     |                  |",
    "|                      ————————————————————————                  |",
    "|                                                                                        |",
    "—————————————————————————————————————————————",
    };

    string menu2[20] =
    {
    "—————————————————————————————————————————————",
    "|               Use W / S to move upwards / downwards, and use y to choose               |",
    "|                                                                                        |",
    "|                      ————————————————————————                  |",
    "|                      |               Start New Game                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                Game Tutorial                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |-------------> History Records <--------------|                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |            Continue previous game            |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                     Exit                     |                  |",
    "|                      ————————————————————————                  |",
    "|                                                                                        |",
    "—————————————————————————————————————————————",
    };

    string menu3[20][91] =
    {
    "—————————————————————————————————————————————",
    "|               Use W / S to move upwards / downwards, and use y to choose               |",
    "|                                                                                        |",
    "|                      ————————————————————————                  |",
    "|                      |               Start New Game                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                Game Tutorial                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |               History Records                |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |----------> Continue previous game <----------|                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                     Exit                     |                  |",
    "|                      ————————————————————————                  |",
    "|                                                                                        |",
    "—————————————————————————————————————————————",
    };

    string menu4[20][91] =
    {
    "—————————————————————————————————————————————",
    "|               Use W / S to move upwards / downwards, and use y to choose               |",
    "|                                                                                        |",
    "|                      ————————————————————————                  |",
    "|                      |               Start New Game                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                Game Tutorial                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |               History Records                |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |            Continue previous game            |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |-------------------> Exit <-------------------|                  |",
    "|                      ————————————————————————                  |",
    "|                                                                                        |",
    "—————————————————————————————————————————————",
    };
    if (index == 0) { cout << menu0[ii][jj]; }
    else if (index == 1) { cout << menu1[ii][jj]; }
    else if (index == 2) { cout << menu2[ii][jj]; }
    else if (index == 3) { cout << menu3[ii][jj]; }
    else if (index == 4) { cout << menu4[ii][jj]; }
    return 0;
}


int show_in_game()
{
    return 0;
}

int show_records()
{
    string header[5] =
    {
    "——————————————————————————————",
    "                                                            ",
    "                        Hitory Records                      ",
    "                                                            ",
    "——————————————————————————————",
    };

    for (int i = 0; i < ui_height; i++)
    {
        for (int j = 0; j < ui_wide; j++)
        {
        if (i == 0 || j == 0 || i == ui_height - 1 || j == ui_wide - 1) { cout << "#"; }//边框 之後再換好看的符號
        else if (4 <= i && i <= 8 && 30 <= j && j <= 89)
        {
            cout << header[i - 4][j - 30];
        }
        else if (10 <= i && i <= 29 && 15 <= j && j <= 104)
        {
            cout << " ";
        }
        else { cout << "0"; } // 其餘位置
        }
        cout << "\n";
    }
    char a;
    a = getch();

    system("cls");
    return 0;
}

void welcomeLoop()
{
	extern int ui_wide;
	extern int ui_height;

    char a;
    cout << "input anything to start" << endl;
    a = getch();

    system("clear");

    int choice;

    string menu0[20] =
    {
    "—————————————————————————————————————————————",
    "|               Use W / S to move upwards / downwards, and use y to choose               |",
    "|                                                                                        |",
    "|                      ————————————————————————                  |",
    "|                      |-------------> Start New Game <---------------|                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                Game Tutorial                 |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |               History Records                |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |            Continue previous game            |                  |",
    "|                      ————————————————————————                  |",
    "|                      ————————————————————————                  |",
    "|                      |                     Exit                     |                  |",
    "|                      ————————————————————————                  |",
    "|                                                                                        |",
    "—————————————————————————————————————————————",
    };

    string pac_man[5] =
    {
    "——————————————————————————————",
    "                                                            ",
    "                           Pac-Man+                         ",
    "                                                            ",
    "——————————————————————————————",
    };


    int index = 0;
    string inputted = "n";
    while (1)
    {
        for (int i = 0; i < ui_height; i++)
        {
        for (int j = 0; j < ui_wide; j++)
        {
            if (i == 0 || j == 0 || i == ui_height - 1 || j == ui_wide - 1) { cout << "#"; }//边框 之後再換好看的符號
            else if (4 <= i && i <= 8 && 30 <= j && j <= 89)
            {
            cout << pac_man[i - 4][j - 30];
            }
            else if (10 <= i && i <= 29 && 15 <= j && j <= 104)
            {
            //cout << menu0[i - 10][j - 15]; 
            welcome_ui(i, j, index); // 歡迎界面：橫向20，縱向90

            }
            else { cout << " "; } // 其餘位置
        }
        cout << "\n";
        }
        inputted = getch();
        if ((inputted == "W" || inputted == "w") && index > 0) { index--; }
        if ((inputted == "S" || inputted == "s") && index < 4) { index++; }


        system("clear");
        if (inputted == "y" || inputted == "Y")
        {
            choice = index;

            switch (choice)
            {
            case 0:
                cout << "We are moving to New game mode" << endl;
                // newgame();
                break;

            case 1:
                cout << "This is the tutorial" << endl;
                // show_tutorial();
                continue;

            case 2:
                show_records();
                continue;

            case 3:
                cout << "We are moving to previous game" << endl;
                // back_to_game();
                break;

            default:
                cout << "We are exiting the game, Press to continue" << endl;
                a = getch();
            }
        }
    }
}

bool gameLoop()
{
    nodelay(stdscr, true); // don't wait until input

    //int score = 0;

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

    int direction = 0;
    while (true)
    {
        int operation = 0;
        
        while (chrono::duration_cast<chrono::milliseconds>(this_frame_time - last_frame_time).count() < time_per_loop)
        {
            int ch = getch();
            if (ch != ERR)
                operation = ch;
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
        
        checkCharacterCollision(pacman, ghosts, turns);

        for (Ghost &ghost: ghosts)
            ghost.move(pacman.x, pacman.y, ghost_speed);
        checkCharacterCollision(pacman, ghosts, turns);
        
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
