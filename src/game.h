#ifndef GAME_H
#define GAME_H

#include <string>
using namespace std;

const double time_per_loop = 400; // milliseconds

extern int score; // declare a score variable that can be modified in other files

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
