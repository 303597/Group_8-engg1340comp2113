#ifndef GAME_H
#define GAME_H

#include <string>
#include <sstream>
using namespace std;

const double time_per_loop = 300; // milliseconds

extern int score; // declare a score variable that can be modified in other files

extern int total_num;

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
    Map* game_map;
    PacMan* pacman; vector<Ghost*> ghosts;
    Menu* game_menu;
    
    bool in_counteratk_mode = false;
    int turns = 0, direction = -2;
    double ghost_speed = 0.4;
    
    int updateTile(int x, int y, string &special, int &prop_turns);
    void showStatus();
    void saveToFile(string filename);
    void checkCharacterCollision();
    
    string special = "none";
    int prop_type = -1, prop_turns = 0, prop_lasting_time = 0;
    int fruit_num = -1, fruit_lasting_time = 0;
    int prop_pos_x = 0, prop_pos_y = 0;
    int fruit_pos_x = 0, fruit_pos_y = 0;
    
public:
    //int cookie_count = 0;
    int level = 0;
    Game(int _level);
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
