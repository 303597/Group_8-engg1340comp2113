#ifndef GAME_H
#define GAME_H

#include <string>
using namespace std;

const double time_per_loop = 400; // milliseconds

extern int score; // declare a score variable that can be modified in other files

template <typename T> T read_line(string line)
{
    istringstream record(line);
    string name;
    T value;
    record >> name >> value;
    return value;
}

class Game
{
private:
    int level;
    Map* game_map;
    PacMan* pacman; vector<Ghost>* ghosts;
    Menu* game_menu;
    
    bool in_counteratk_mode = false;
    int turns = 0;
    double ghost_speed = 0.4;
    
    int updateTile(int x, int y, string &special, int &prop_turns);
    void showStatus();
    void saveToFile(string filename);
    void checkCharacterCollision();
    
    int prop_type = -1, prop_turns = 0, prop_lasting_time = 0;
    int fruit_num = -1, fruit_lasting_time = 0;
    int prop_pos_x = game_map->vals.size() - 1, prop_pos_y = game_map->vals[prop_pos_x].size() - 1;
    int fruit_pos_x = game_map->vals.size() - 1, fruit_pos_y = game_map->vals[fruit_pos_x].size() - 1;
    
public:
    Game(int level);
    Game(string filename);
    int startGame();
    int pauseGame();
};

void save(); // declare the save function

int welcomeLoop();

void initializeGame(string filename = "");

//bool isMapCompleted(const string&);

//int gameLoop(int level, Map &game_map, PacMan &pacman, vector<Ghost> &ghosts);

void showTutorial();

void showHighScore();

#endif
