#include "tools.h"
#include "characters.h"
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <cursesw.h>

Character::Character(int _start_x, int _start_y)
{
	start_x = x = _start_x;
	start_y = y = _start_y;
}

void Character::linkMap(Map *_map)
{
	map = _map;
}

void PacMan::move(int direction, string special)
{
	// note: check collision

	int dirx[] = {-1, 1, 0, 0};
	int diry[] = {0, 0, -1, 1};

	int x_temp, y_temp;
	x_temp = x + dirx[direction];
	y_temp = y + diry[direction];
	if (x_temp < 0)
	{
		x_temp += map->vals.size();
	}
	else if (x_temp > map->vals.size() - 1)
	{
		x_temp -= map->vals.size();
	}
	if (y_temp < 0)
	{
		y_temp += map->vals[x].size();
	}
	else if (y_temp > map->vals[x].size() - 1)
	{
		y_temp -= map->vals[x].size();
	}
	// deal with overflow;
	if(special == "pass_bricks")
	{
		x = x_temp;
		y = y_temp;
		return;
	}
	if (map->vals[x_temp][y_temp] != '#' && map->vals[x_temp][y_temp] != '_')
	{
		x = x_temp;
		y = y_temp;
	}
	return;
}

void Ghost::move(int target_x, int target_y, double speed)
{
	if(x == start_x && y == start_y){
		if(map->vals[x-1][y] == '_'){
			x = x - 1; y = y;
		}
		else{
			x = x + 1; y = y;
		}
	else if(map->vals[x][y] == '_'){
		if(x+1 == start_x)
			x = x - 1; y = y;
		}
		else{
			x = x + 1; y = y;
		}
		//return;
	}//initializing
	
	int co_x, co_y; // corresponding postions
	int fl = 0, fx = 1, fy = 1;
	int poss[4] = {1, 1, 1, 1}; // up down left right
	int dirx[4] = {-1, 1, 0, 0};
	int diry[4] = {0, 0, -1, 1}; // up down left right
	if (in_counteratk_mode)
	{
		for (int i = 0; i < 4; i++)
		{
			dirx[i] *= -1;
			diry[i] *= -1;
		}
	}
	for (int p = 0; p < 4; p++)
	{
		int xx = x + dirx[p];
		int yy = y + diry[p];
		if (xx < 0)
		{
			xx = map->vals.size() - 1;
		}
		else if (xx > map->vals.size()-1)
		{
			xx = 0;
		}
		if (yy < 0)
		{
			yy = map->vals[x].size() - 1;
		}
		else if (yy > map->vals[x].size() - 1)
		{
			yy = 0;
		}
		// deal with overflow;
		if (map->vals[xx][yy] == '#' || map->vals[xx][yy] == '_')
		{
			poss[p] = 0;
		}
	}
	// delete impossible moving directions;
	co_x = x - target_x;
	co_y = y - target_y;
	// corresponding direction of the pacman to ghost
	if(in_counteratk_mode)
	{
		int number = rand();
		if (co_y == 0)
		{
			if (poss[2] != 0 && poss[3] != 0)
			{
				if (number % 2 == 0)
				{
					y += 1;
				}
				else
				{
					y -= 1;
				}
				fl = 1;
			}
			else if (poss[3] != 0 && poss[2] == 0)
			{
				y -= 1;
				fl = 1;
			}
			else if (poss[3] == 0 && poss[2] != 0)
			{
				y += 1;
				fl = 1;
			}
		}
		else if (co_x == 0)
		{
			if (poss[0] != 0 && poss[1] != 0)
			{
				if (number % 2 == 0)
				{
					x += 1;
				}
				else
				{
					x -= 1;
				}
				fl = 1;
			}
			else if (poss[0] != 0 && poss[1] == 0)
			{
				x += 1;
				fl = 1;
			}
			else if (poss[0] == 0 && poss[1] != 0)
			{
				x -= 1;
				fl = 1;
			}
		}
	}// better avoid pac-man
	if (fl == 1)
	{
		if (x < 0)
		{
			x += map->vals.size();
		}
		else if (x > map->vals.size() - 1)
		{
			x -= map->vals.size();
		}
		if (y < 0)
		{
			y += map->vals[x].size();
		}
		else if (y > map->vals[x].size() - 1)
		{
			y -= map->vals[x].size();
		}
		// deal with overflow;
		return;
	}
	// if in the same line-- better track or avoid the pac-man.
	int verti, hori; // up or down ; left or right
	if (co_x <= 0)
	{
		verti = 1;
	}
	else if (co_x > 0)
	{
		verti = 0;
	}
	if (co_y <= 0)
	{
		hori = 3;
	}
	else if (co_y > 0)
	{
		hori = 2;
	}
	int sum=0;
	for(int p = 0; p < 4; p++){
		if(poss[p] == 0){
			continue;
		}
		if(verti == p)
		{
			if(co_y == 0)
			{
				if(map->vals[0][y] != '#' && map->vals[map->vals.size()-1][y] != '#')
				{
					if(abs(co_x) > map->vals.size() / 2)
					{
						if(poss[1-p] != 0)
						{
							poss[1-p] = 20 + sum; sum += 20;
						}
						poss[p] = 3 + sum; sum += 3;
						continue;
					}// better chase the pac-man
				}
				poss[p] = 103 + sum; sum += 103;
			}
			else
			{
				poss[p] = 10 + sum; sum += 10;
			}
		}
		else if (hori == p)
		{
			if(co_x == 0)
			{
				if(map->vals[x][0] != '#' && map->vals[x][map->vals[x].size()-1] != '#')
				{
					if(abs(co_y) > map->vals[x].size() / 2)
					{
						if(poss[5-p] != 0)
						{
							poss[5-p] = 20 + sum; sum += 20;
						}
						poss[p] = 3 + sum; sum += 3;
						continue;
					}// better chase the pac-man
				}
				poss[p] = 103 + sum; sum += 103;
			}// better chase the pac-man
			else
			{
				poss[p] = 20 + sum; sum += 20;
			}
		}
		else{
			poss[p] = 1 + sum; sum += 1;
		}
	}
	int num = rand() % sum; // generate a random number range [0, sum-1]
	for(int p = 0; p < 4; p++){
		if(num < poss[p]){
			x += dirx[p];
			y += diry[p];
			break;
		}
	}
	if (x < 0)
	{
		x += map->vals.size();
	}
	else if (x > map->vals.size() - 1)
	{
		x -= map->vals.size();
	}
	if (y < 0)
	{
		y += map->vals[x].size();
	}
	else if (y > map->vals[x].size() - 1)
	{
		y -= map->vals[x].size();
	}
	// deal with overflow;
	return;
}

void checkCharacterCollision(PacMan &pacman, vector<Ghost> &ghosts, int &turns, int &direction, int &prop_lasting_time, int &fruit_lasting_time, int &prop_turns, string &special)
{
	for (int i = 0; i < ghosts.size(); ++i)
	{
		if (ghosts[i].x == pacman.x && ghosts[i].y == pacman.y)
		{
			// determine the mode of the ghost
			if (ghosts[i].in_counteratk_mode)
			{
				pacman.eaten_ghosts++;
				if(special == "double_points")
				{
					score += 100 * pacman.eaten_ghosts;
				}
				else
				{
					score += 50 * pacman.eaten_ghosts; // modify the extern score variable directly
				}
				ghosts[i].x = ghosts[i].start_x;
				ghosts[i].y = ghosts[i].start_y;
				ghosts[i].in_counteratk_mode = false;
				ghosts[i].speed -= 2;
				// the eaten ghost reset to normal mode
			}
			else
			{
				pacman.lives--;
				direction = -2;
				if (pacman.lives == 0)
				{
					//mvprintw(12, 12, "%d", score);
					refresh();
					nodelay(stdscr, false);
					mvprintw(13, 12, "Press any key to quit.");
					getch();
					return; // player dies
				}
				pacman.x = pacman.start_x;
				pacman.y = pacman.start_y;
				for (int j = 0; j < ghosts.size(); ++j)
				{
					ghosts[j].x = ghosts[j].start_x;
					ghosts[j].y = ghosts[j].start_y;
					ghosts[j].in_counteratk_mode = false;
					ghosts[j].speed -= 2;
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
		if(ghosts[i].in_counteratk_mode == true)
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
