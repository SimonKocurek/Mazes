#include "sight.h" // header
#include <stdlib.h>

char** visible;
char initilized = 0;

void process_sight(void) {
	if (!initilized) {
		visible = malloc ((size_t)size.y * sizeof(char*));
		for (int i = 0; i < size.y; i++)
			visible[i] = malloc((size_t)size.x * sizeof(char));

		initilized = 1;
	}

	// base
	for (int y = 0; y < size.y; y++)
		for (int x = 0; x < size.x; x++) {
			if (abs(player.x -x) < 5 && abs(player.y - y) < 5) 
				if (abs(player.x -x) + abs(player.y - y) <= 5) {
					visible[y][x] = 1;
					continue;
				}
			visible[y][x] = 0;
		}

	// goal
	for (int y = 0; y < size.y; y++)
		for (int x = 0; x < size.x; x++) {
			if (maze[y][x] == 'X') visible[y][x] = 1;
			else continue;

			if (y+1 < size.y) visible[y+1][x] = 1;
			if (y-1 >= 0) visible[y-1][x] = 1;
			if (x+1 < size.x) visible[y][x+1] = 1;
			if (x-1 >= 0) visible[y][x-1] = 1;
		}

	// line of sight
	point offset = {0,0};
	switch(direct) {
		case UP: offset.y = -1; break;
		case DOWN: offset.y = 1; break;
		case LEFT: offset.x = -1; break;
		case RIGHT: offset.x = 1; break;
	}
	point sght;
	sght.x = player.x;
	sght.y = player.y;

	while(maze[sght.y][sght.x] != '#' && maze[sght.y][sght.x] != 'X') {
		sght.x += offset.x;
		sght.y += offset.y;

		visible[sght.y][sght.x] = 1;
		if (sght.y +1 < size.y) visible[sght.y+1][sght.x] = 1;
		if (sght.y -1 >= 0) visible[sght.y-1][sght.x] = 1;
		if (sght.x +1 < size.x) visible[sght.y][sght.x+1] = 1;
		if (sght.x -1 >= 0) visible[sght.y][sght.x-1] = 1;

		if (sght.y +2 < size.y) visible[sght.y+2][sght.x] = 1;
		if (sght.y -2 >= 0) visible[sght.y-2][sght.x] = 1;
		if (sght.x +2 < size.x) visible[sght.y][sght.x+2] = 1;
		if (sght.x -2 >= 0) visible[sght.y][sght.x-2] = 1;
	}
	if (sght.y +1 < size.y && sght.x +1 < size.x) 
		visible[sght.y+1][sght.x+1] = 1;
	if (sght.y -1 >= 0 && sght.x +1 < size.x) 
		visible[sght.y-1][sght.x+1] = 1;
	if (sght.y +1 < size.y && sght.x -1 >= 0) 
		visible[sght.y+1][sght.x-1] = 1;
	if (sght.y -1 >= 0 && sght.x -1 >= 0) 
		visible[sght.y-1][sght.x-1] = 1;
}

char is_visible(point* place) {
	return visible[place->y][place->x];
}

void free_sight(void) {
	initilized = 0;

	for	(int i = 0; i < size.y; i++)
		free(visible[i]);
	free(visible);
}

