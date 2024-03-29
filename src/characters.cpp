#include "tools.h"
#include "characters.h"
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <cursesw.h>

typedef map<pair<int, int>, int> DirectionMap;

// Creates a character with starting coordinates
Character::Character(int _start_x, int _start_y)
{
	start_x = x = _start_x;
	start_y = y = _start_y;
}

void Character::linkMap(Map *_map)
{
	map = _map;
}

// This function realizes move of Pacman and deals with situation of overflow
void PacMan::move(int direction, string special)
{
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

// This function realizes the move of ghosts - chase after Pacman in normal mode and run away in counter-attack mode
void Ghost::move(int target_x, int target_y, int ghost_num)
{
	if(x == start_x && y == start_y){
		if(map->vals[x-1][y] == '_'){
			x = x - 1; y = y;
		}
		else{
			x = x + 1; y = y;
		}
		return;
	}
	else if(map->vals[x][y] == '_'){
		if(x + 1 == start_x){
			x = x - 1; y = y;
		}
		else{
			x = x + 1; y = y;
		}
		return;
	}
	//initializing
	
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
							poss[1-p] = 10 * ghost_num + sum; sum += 10 * ghost_num;
						}
						poss[p] = 2 * ghost_num + sum; sum += 2 * ghost_num;
						continue;
					}// better chase the pac-man
				}
				poss[p] = 30 * ghost_num + sum; sum += 30 * ghost_num;
			}
			else
			{
				poss[p] = 8 * ghost_num + sum; sum += 5 * ghost_num;
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
							poss[5-p] = 10 * ghost_num + sum; sum += 10 * ghost_num;
						}
						poss[p] = 2 * ghost_num + sum; sum += 2 * ghost_num;
						continue;
					}// better chase the pac-man
				}
				poss[p] = 30 * ghost_num + sum; sum += 30 * ghost_num;
			}// better chase the pac-man
			else
			{
				poss[p] = 8 * ghost_num + sum; sum += 8 * ghost_num;
			}
		}
		else{
			poss[p] = ghost_num + sum; sum += ghost_num;
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

bool Ghost::moveBFS(int target_x, int target_y)
{
	int dirx[4] = {-1, 1, 0, 0};//up down left right
    int diry[4] = {0, 0, -1, 1};
	if (x == target_x && y == target_y)
		return true;
	queue<pair<int, int>> q;
	q.push(make_pair(x, y));
	int steps = 0;
	bool found = false;
	DirectionMap directions;
	directions[make_pair(x, y)] = -1;
	
	//mvprintw(36, 0, "%d %d", x, y);
	while (!q.empty() && steps < map->size().first * map->size().second && !found)
	{
		pair<int, int> now = q.front();
		q.pop();
		steps++;
		for (int i = 0; i < 4; i++)
		{
			pair<int, int> nxt = make_pair(now.first + dirx[i], now.second + diry[i]);
			if (nxt.first >= 0 && nxt.first < map->size().first && nxt.second >= 0 && nxt.second < map->size().second 
				&& map->vals[nxt.first][nxt.second] != '#' && directions.count(nxt) == 0)
			{
				if (directions[now] == -1)
					directions[nxt] = i;
				else
					directions[nxt] = directions[now];
				if (nxt.first == target_x && nxt.second == target_y)
				{
					found = true;
					break;
				}
				q.push(nxt);
			}
		}
	}
	if (found)
	{
		int move_dir = directions[make_pair(target_x, target_y)];
		x += dirx[move_dir];
		y += diry[move_dir];
		//mvprintw(36, 10, "%d %d", found, move_dir);
	}
	//mvprintw(36, 10, "%d", found);
	return found;
}

// Show expressions of pacman (change with items)
void PacMan::show(string special, bool in_counteratk_mode)
{
	int posx = x + 2, posy = 2 * y + 4;
	string expression;
	if (in_counteratk_mode)
		expression = "😈";
	else if (special == "magnet")
		expression = "🤑";
	else if (special == "slow")
		expression = "🤓";
	else if (special == "pass_bricks")
		expression = "🤩";
	else
	{
		if (expression_timer >= 20)
		{
			expression_timer = 0;
			expression_index = rand() % 5;
		}
		expression = expression_list[expression_index];
		expression_timer++;
	}
	mvprintw(posx, posy, "%s", expression.c_str());
}

void Ghost::show()
{
	if (in_counteratk_mode)
		printElement(x, y, 'e');
	else
		printElement(x, y, 'E');
}

//void checkCharacterCollision(PacMan &pacman, vector<Ghost> &ghosts, int &turns, int &direction, int &prop_lasting_time, int &fruit_lasting_time, int &prop_turns, string &special)
