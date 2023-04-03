#ifndef GAME_H
#define GAME_H

const double time_per_loop = 400; // milliseconds

extern int score; // declare a score variable that can be modified in other files

void save(); // declare the save function

void welcomeLoop();

bool gameLoop();

#endif
