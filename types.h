#ifndef TYPES
#define TYPES

#define TIMEOUT 850

#define min_x 15
#define min_y 15

#define max_x 65
#define max_y 55 

typedef struct point_def {
    int x;
    int y;
} point;

typedef enum direction_def {UP, DOWN, LEFT, RIGHT} direction;

extern point size;
extern int** maze;

// player
extern point player;
extern direction direct;

// trolls
extern int trolls;
extern point* troll;
extern char* troll_alive;

// portals
extern int portals;
extern point* portal;
extern point* portal_exit;

// score
extern int wins;
extern int loses;

#endif
