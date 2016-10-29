#ifndef MOVEMENT
#define MOVEMENT

#include "types.h"

void change_direction(int key);
void move_player(int key);
void move_troll(int id);
void portal_teleport(void);
int point_distance(point a, point b);

#endif
