#ifndef GAME_H
#define GAME_H

const double time_per_loop = 400; // milliseconds

extern int score; // declare a score variable that can be modified in other files

void save(); // declare the save function

int welcomeLoop();

bool gameLoop(int level);

void showTutorial();

void showHighScore();

#endif
