#include "tools.h"
#include "characters.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>

Character::Character(int _start_x, int _start_y)
{
    start_x = x = _start_x; start_y = y = _start_y;
}

void Character::linkMap(Map* _map)
{
    map = _map;
}

void PacMan::move(int direction)
{
    // note: check collision

    int dirx[] = {-1, 1, 0, 0};
    int diry[] = {0, 0, -1, 1};

    int x_temp, y_temp;
    x_temp = x + dirx[direction]; y_temp = y + diry[direction];

    if (map->vals[x_temp][y_temp] != '#')
    {
        x = x + dirx[direction]; y = y + diry[direction];
    }
}

void Ghost::move(int target_x, int target_y, double speed)
{
    int co_x,co_y;//corresponding postions
	int multi = 1; // switch mode 
	int poss[4] = {1, 1, 1, 1}, cnt=4; // up down right left
	int dirx[4] = {0, 0, 1, -1};
	int diry[4] = {-1, 1, 0, 0};// up down right left
	if(in_counteratk_mode){
		for(int i = 0; i < 4; i++){
			dirx[i] *= -1;
			diry[i] *= -1;
		}
	}
	srand(time(0));
	int num = rand() % 12; //generate a random number range [0, 11]
	for(int i = 0; i < (*ghosts).size(); i++){
		for(int p = 0; p < 4; p++){
			int xx = (*ghosts)[i].x + dirx[p];
			int yy = (*ghosts)[i].y + diry[p];
			if(xx < 0){ xx = map.map.vals.size() - 1; }
			else if(xx > map.vals.size()){ xx = 0; }
			if(yy < 0){ xx = map.vals[i].size() - 1; }
			else if(yy > map.vals[i].size()){ xx = 0; }
			// deal with overflow;
			if(map.vals[xx][yy] == '#'){ poss[i] = 0; cnt--; }
		}
		// delete impossible moving directions; divide possibilities
		co_x = (*ghosts)[i].x - target_x;
    	co_y = (*ghosts)[i].y - target_y;
    	// corresponding direction of the pacman to ghost
    	if(co_x == 0){
	    	if(co_y < 0){ (*ghosts)[i].y += speed; }
			else if (co_y > 0){ (*ghosts)[i].y -= speed; }
			continue;
		}
		if(co_y == 0){
	    	if(co_x < 0){ (*ghosts)[i].x += speed; }
			else if (co_x > 0){ (*ghosts)[i].x -= speed; }
			continue;
		}
		// if in line-- better track the pac-man.
		int hori, verti, type; // up or down ; left or right
		if( co_x < 0 ) { verti = 1; }
		else if (co_x > 0 ){verti = 0;}
		if(co_y < 0) {hori = 2;}
		else if (co_y > 0){hori = 3;}
		if(cnt == 1){type = 1;}
		else if(cnt == 2){
			if((poss[hori] == 0 && poss[verti] != 0) || (poss[hori] != 0 && poss[verti] == 0)){type = 3;}
			else{type = 2;}
		}
		else if(cnt == 3){
			if((poss[hori] == 0 && poss[verti] != 0) || (poss[hori] != 0 && poss[verti] == 0)){type = 4;}
			else if(poss[hori] != 0 && poss[verti] != 0){type = 5;}
		}
		else{type = 6;}
		// 12:0 6:6 10:2 8:2:2 5:5:2 5:5:1:1			
		switch (type)
		{
			case 1:
				for(int p = 0; p < 4; p++){
					if(poss[i] != 0){(*ghosts)[i].x += dirx[p]*speed; (*ghosts)[i].y += diry[p]*speed; break; }
				}
				break;
			case 2:
				for(int p = 0; p < 4; p++){
					if(poss[i] != 0){
						if(num % 2 == 0){(*ghosts)[i].x += dirx[p]*speed; (*ghosts)[i].y += diry[p]*speed; break; }
					}
					else{
						num += 1; 
						continue;
					}
				}
				break;
			case 3:
				if(poss[hori] == 0 && poss[verti] != 0){
					if(num <= 9 && num >= 0){
						(*ghosts)[i].x += dirx[verti]*speed; (*ghosts)[i].y += diry[verti]*speed;	
					}
					else{
						(*ghosts)[i].x += dirx[5-hori]*speed; (*ghosts)[i].y += diry[5-hori]*speed;
					}
				}
				else{
					if(num <= 9 && num >= 0){
						(*ghosts)[i].x += dirx[hori]*speed; (*ghosts)[i].y += diry[hori]*speed;
					}
					else{
						(*ghosts)[i].x += dirx[3-verti]*speed; (*ghosts)[i].y += diry[3-verti]*speed;
					}
				}
				break;
			case 5:
				if(num <= 4 && num >= 0) {
					(*ghosts)[i].x += dirx[verti]*speed; (*ghosts)[i].y += diry[verti]*speed; 
				}
				else if(num <=9 && num >-5){
					(*ghosts)[i].x += dirx[hori]*speed; (*ghosts)[i].y += diry[hori]*speed; 
				}
				else{
					if(poss[5-hori] != 0){
						(*ghosts)[i].x += dirx[5-hori]*speed; (*ghosts)[i].y += diry[5-hori]*speed;
					}
					else{
						(*ghosts)[i].x += dirx[3-verti]*speed; (*ghosts)[i].y += diry[3-verti]*speed;
					}
				}
				break;
			case 4:
				if(num >= 0 && num <= 7){
					if(poss[hori] == 0 && poss[verti] != 0){
						(*ghosts)[i].x += dirx[verti]*speed; (*ghosts)[i].y += diry[verti]*speed;
					}
					else{
						(*ghosts)[i].x += dirx[hori]*speed; (*ghosts)[i].y += diry[hori]*speed;
					}
				}
				else if(num >= 8 && num <=9){
					(*ghosts)[i].x += dirx[5-hori]*speed; (*ghosts)[i].y += diry[5-hori]*speed;
				}
				else{
					(*ghosts)[i].x += dirx[3-verti]*speed; (*ghosts)[i].y += diry[3-verti]*speed;
				}
				break;
			case 6:
				if(num <= 4 && num >= 0){
					(*ghosts)[i].x += dirx[verti]*speed; (*ghosts)[i].y += diry[verti]*speed;
				}
				else if(num <= 9 && num >= 5){
					(*ghosts)[i].x += dirx[hori]*speed; (*ghosts)[i].y += diry[hori]*speed; 
				}
				else if(num == 10){
					(*ghosts)[i].x += dirx[5-hori]*speed; (*ghosts)[i].y += diry[5-hori]*speed;
				}
				else{
					(*ghosts)[i].x += dirx[3-verti]*speed; (*ghosts)[i].y += diry[3-verti]*speed;
				}
				break;		
		}
		// if in line-- better avoid the pac-man.
		if((*ghosts)[i].x < 0){ (*ghosts)[i].x += map.vals.size(); }
		else if((*ghosts)[i].x > map.vals.size()){ (*ghosts)[i].x -= map.vals.size(); }
		if((*ghosts)[i].y < 0){ (*ghosts)[i].y += map.vals[i].size(); }
		else if((*ghosts)[i].y > map.vals[i].size()){ (*ghosts)[i].y -= map.vals.size(); }
		// deal with overflow;
	}
    return;
}

void checkCharacterCollision(PacMan &pacman, vector<Ghost> &ghosts)
{   
    int score;
    for (int i=0;i<ghosts.size();++i)
    {
        if (ghosts[i].x==pacman.x && ghosts[i].y==pacman.y)
        {
            // determine the mode of the ghost
            if (ghosts[i].in_counteratk_mode)
            {
                pacman.eaten_ghosts++;
                score += 50*pacman.eaten_ghosts;
                ghosts[i].x = ghosts[i].start_x;
                ghosts[i].y = ghosts[i].start_y;
                // the eaten ghost reset
            }
            else
            {
                pacman.lives--;
                if (pacman.lives == 0) return; // player dies
                pacman.x = pacman.start_x;
                pacman.y = pacman.start_y;
                for (int j=0; j<ghosts.size(); ++j)
                {
                    ghosts[j].x = ghosts[j].start_x;
                    ghosts[j].y = ghosts[j].start_y;
                }
                // player and all the ghosts reset
            }
        }
    }
    // note: check counteratk mode, handle eat results
}
