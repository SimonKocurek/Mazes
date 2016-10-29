#include "spawner.h" // header

#include "types.h" // global variables
#include <time.h> // seed from time
#include <stdlib.h> // rand
#include "movement.h" // point_distance

void initialize_rand(void) {
    srand((unsigned int)time(NULL));
}

int get_random(int x) {
    return rand() % x;
}

void free_spawns(void) {
    free( troll );
    free( troll_alive );

    free( portal );
	free( portal_exit );
}

char player_point(int x, int y) {
    point temp = {x,y};
    if (point_distance(temp, player) < 10)
	return 1;
    return 0;
}

// false means dead end
char dead_end(int x, int y) {
    int walls = 0;
    if (maze[y+1][x] == '#') walls++;
    if (maze[y-1][x] == '#') walls++;
    if (maze[y][x+1] == '#') walls++;
    if (maze[y][x-1] == '#') walls++;

    if (walls != 3) return 1;
    else return 0;
}

void find_free_spaces(point* tiles, int* filled, char check, char (*place) (int, int)) {
    for (int y = 1; y < size.y-1; y ++) 
        for (int x = 1; x < size.x-1; x ++) 
            if (maze[y][x] == ' ') {
				if (check && place(x, y))
					continue;

				tiles[*filled].x = x;
				tiles[*filled].y = y;
				(*filled) ++; 
            }
}

void place_portals(int count) {
    portals = count;
    portal = malloc( 2 * (size_t)portals * sizeof(point) );
	portal_exit = malloc( 2 * (size_t)portals * sizeof(point) );
	for (int i = 0; i < portals*2; i++) {
		portal_exit[i].x = 0;
		portal_exit[i].y = 0;
	}

    point tiles[size.x * size.y];
    int filled = 0;

    find_free_spaces(tiles, &filled, 1, dead_end);

    for (int i = 0; i < portals*2; i ++) {
        int random_tile;
		while (1) {
			random_tile = rand() % filled;
			if (tiles[random_tile].x != -1)
				break;
		}
		

		portal[i].x = tiles[random_tile].x;
		portal[i].y = tiles[random_tile].y;

		if (maze[portal[i].y][portal[i].x+1] == ' ') portal_exit[i].x = 1;
		if (maze[portal[i].y][portal[i].x-1] == ' ') portal_exit[i].x = -1;
		if (maze[portal[i].y+1][portal[i].x] == ' ') portal_exit[i].y = 1;
		if (maze[portal[i].y-1][portal[i].x] == ' ') portal_exit[i].y = -1;

		tiles[random_tile].x = -1;
    }   
}

void place_trolls(int count) {
    trolls = count;

    troll = malloc( (size_t)trolls * sizeof(point));
    troll_alive = malloc( (size_t)trolls );

    point tiles[size.x * size.y];
    int filled = 0;
    
    find_free_spaces(tiles, &filled, 1, player_point);

    for (int i = 0; i < trolls; i ++) {
        int random_tile;
		while (1) {
			random_tile = rand() % filled;
			if (tiles[random_tile].x != -1)
				break;
		}
		

		troll[i].x = tiles[random_tile].x;
		troll[i].y = tiles[random_tile].y;
		troll_alive[i] = 1;

		tiles[random_tile].x = -1;
    }   
}

void place_player(void) {
    point tiles[size.x * size.y];
    int filled = 0;

    find_free_spaces(tiles, &filled, 0, player_point);

    int random_tile = rand() % filled;
    player.x = tiles[random_tile].x;
    player.y = tiles[random_tile].y;
}

