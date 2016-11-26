#include "movement.h" // header

#include <ncurses.h> // getch
#include <limits.h> // INT_MAX/ INT_MIN
#include <stdlib.h> // abs

typedef struct path_point_def {
    point previous;
    int f_score;
    int g_score;
    char visited;
} path_point;

void bounds_check(point* place) {
    if (place->x < 0) place->x = 0;
    if (place->x >= size.x) place->x = size.x -1;
    
    if (place->y < 0) place->y = 0;
    if (place->y >= size.y) place->y = size.y -1;
}

int point_distance(point a, point b) {
    return abs(a.x - b.x) + abs(a.y - b.y); 
}

point* pathfider_path_step(path_point** tile, point* current) {
    while (tile[current->y][current->x].g_score > 1) {
		current = &(tile[current->y][current->x].previous);
    }

    return current;
}

point* pathfider(point* start, point goal) {
    path_point** tile = malloc( (size_t)size.y * sizeof(path_point*) );
    for (int i = 0; i < size.y; i++)
		tile[i] = malloc( (size_t)size.x * sizeof(path_point) );

    point possible[size.x * size.y];

    // used at most
    int possible_size = 1;
    // not deleted
    int possible_count = 1;
    possible[0] = *start;

    for (int y = 0; y < size.y; y++)
		for (int x = 0; x < size.x; x++) {
			tile[y][x].f_score = INT_MAX;
			tile[y][x].g_score = INT_MAX;
			tile[y][x].visited = 0;
	}

    tile[start->y][start->x].g_score = 0;
    tile[start->y][start->x].f_score = point_distance(*start, goal);

    // while has somewhere to go
    while (possible_count > 0) {
		// find closest to goal
		int min = INT_MAX;
		int selected_id;
		point selected;
		for (int i = 0; i < possible_size; i++) {
			// skip removed
			if (possible[i].x == INT_MIN)
				continue;

			point selected_temp = possible[i];
			if (tile[selected_temp.y][selected_temp.x].f_score < min) {
				selected = selected_temp;
				selected_id = i;
				min = tile[selected.y][selected.x].f_score;
			}
		}

		// goal reached
		if (selected.y == goal.y && selected.x == goal.x) {
			point* temp = pathfider_path_step(tile, &goal);
			point* result = malloc(sizeof(point));
			result->x = temp->x;
			result->y = temp->y;

			for (int i = 0; i < size.y; i++)
				free(tile[i]);
			free(tile);

			return result;
		}

		// remove block from possible
		// if x is INT_MIN -> removed point
		tile[selected.y][selected.x].visited = 1;
		possible[selected_id].x = INT_MIN;
		possible_count--;

		// for each neighbour
		for (int x, y, i = 0; i < 4; i++) {
			switch (i) {
				case 0: x = selected.x -1; y = selected.y; break;
				case 1: x = selected.x +1; y = selected.y; break;
				case 2: x = selected.x; y = selected.y -1; break;
				case 3: x = selected.x; y = selected.y +1; break;
			}

			point neighbour = {x,y};
			bounds_check(&neighbour);
			path_point* neighbour_t = tile[neighbour.y] + neighbour.x;

			// ignore walls
			if (maze[neighbour.y][neighbour.x] == '#')
				continue;
		   
			// ignore visited neighbours 
			if (neighbour_t->visited)
				continue;
			
			// calculate score
			int selected_g_score = tile[selected.y][selected.x].g_score;
			int score = selected_g_score + 1; 

			// add to possible if not already
			char possible_bool = 1;
			for (int j = 0; j < possible_size; j++) {
				if (possible[j].x == neighbour.x && possible[j].y == neighbour.y) {
					possible_bool = 0;
					break;
				}
			}

			if (possible_bool) {
				possible[possible_size] = neighbour;
				possible_size++;
				possible_count++;
			} else {
				if (score >= neighbour_t->g_score)
				continue;
			}

			neighbour_t->previous = selected;
			neighbour_t->g_score = score;
			neighbour_t->f_score = score + point_distance(neighbour, goal);
		}
    }

    // deallocate
    for (int i = 0; i < size.y; i++)
		free(tile[i]);
    free(tile);

    // fail
	point* temp = malloc(sizeof(point));
	*temp = *possible;
    return temp;
}

void move_troll(int id) {
    point* next_tile = pathfider(troll + id, player);

	// bug fix :/ 
	if(next_tile->x > 0)
		troll[id].x = next_tile->x;
    troll[id].y = next_tile->y;

	free(next_tile);
}

void move_player(int key) {
    if (!(key == 'w' || key == 's' || key == 'a' || key == 'd') && !(key == 'i' || key == 'k' || key == 'j' || key == 'l'))
		return;

    point place = {player.x, player.y};
    point offset = {0, 0};

    switch (direct) {
        case UP: offset.y --; break;
        case DOWN: offset.y ++; break;
        case LEFT: offset.x --; break;
        case RIGHT: offset.x ++; break;
    }

    place.x += offset.x;
    place.y += offset.y;
    bounds_check(&place);

    if (maze[place.y][place.x] != '#') {
		// move to non-wall location
		player.x = place.x;
		player.y = place.y;
    } else  {
		// move block and move to it's location
		point previous = {place.x, place.y};

    	place.x += offset.x;
		place.y += offset.y;
		bounds_check(&place);

		if (maze[place.y][place.x] != '#') {
			player.x = previous.x;
			player.y = previous.y;

			maze[previous.y][previous.x] = ' ';

			// don't overwrite goal
			if (maze[place.y][place.x] != 'X')
				maze[place.y][place.x] = '#';

			// kill troll
			for (int i = 0; i < trolls; i ++) {
				if (troll[i].x == place.x && troll[i].y == place.y) {
					troll_alive[i] = 0;
				}
			}
		}
    }

}

void portal_teleport(void) {
	// forward portals
	for (int i = 0; i < portals; i++) {
		// if not blocked
		if (maze[portal[i+portals].y][portal[i+portals].x] == '#')
			continue;

		if (player.x == portal[i].x && player.y == portal[i].y) {
			player.x = portal[i+portals].x + portal_exit[i+portals].x;
			player.y = portal[i+portals].y + portal_exit[i+portals].y;
		}

		for (int j = 0; j < trolls; j++) {
			if (troll[j].x == portal[i].x && troll[j].y == portal[i].y) {
				troll[j].x = portal[i+portals].x + portal_exit[i+portals].x;
				troll[j].y = portal[i+portals].y + portal_exit[i+portals].y;
			}
		}
	}

	// back portals
	for (int i = portals; i < portals*2; i++) {
		// if not blocked
		if (maze[portal[i-portals].y][portal[i-portals].x] == '#')
			continue;

		if (player.x == portal[i].x && player.y == portal[i].y) {
			player.x = portal[i-portals].x + portal_exit[i-portals].x;
			player.y = portal[i-portals].y + portal_exit[i-portals].y;
		}

		for (int j = 0; j < trolls; j++) {
			if (troll[j].x == portal[i].x && troll[j].y == portal[i].y) {
				troll[j].x = portal[i-portals].x + portal_exit[i-portals].x;
				troll[j].y = portal[i-portals].y + portal_exit[i-portals].y;
			}
		}
	}
}

void change_direction(int key) {
    switch (key) {
        case 'w': direct = UP; return;
        case 's': direct = DOWN; return;
        case 'a': direct = LEFT; return;
        case 'd': direct = RIGHT; return;

        case 'i': direct = UP; return;
        case 'k': direct = DOWN; return;
        case 'j': direct = LEFT; return;
        case 'l': direct = RIGHT; return;
    }
}

