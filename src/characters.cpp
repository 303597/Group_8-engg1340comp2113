#include "tools.h"
#include "characters.h"

Character::Character(int start_x, int start_y, Map* on_map)
{
    x = start_x; y = start_y; map = on_map;
}

void PacMan::move(int direction)
{
    // not implemented
}

void Ghost::move(int target_x, int target_y, bool in_counteratk_mode)
{
    // not implemented
}