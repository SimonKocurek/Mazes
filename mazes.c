#include "mazes.h" // header

#include "types.h" // global variables
#include "spawner.h" // random
#include "stack.h"
#include "messages.h" // simple_offset
#include <stdlib.h> // malloc

#include <ncurses.h>
#include <unistd.h> // usleep

void initialize_maze(void) {
    size.x = min_x + get_random(max_x - min_x + 1);
    size.y = min_y + get_random(max_y - min_y + 1);

    size.x += size.x % 2 + 1;

    maze = malloc((size_t)size.y * sizeof(int*));
    for (int y = 0; y < size.y; y++)
		maze[y] = malloc((size_t)(size.x + 1) * sizeof(int)); 
}

void free_maze(void) {
    for (int y = 0; y < size.y; y++)
		free(maze[y]);
    free(maze);
}

void fill_walls(void) {
    for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++)
			maze[y][x] = '#';
		maze[y][size.x] = '\0';
    }
}

point* set_goal(void) {
    point start = {0, 0};
    point offset = {0, 0};
    
    switch(get_random(4)) {
		case 0: offset.x = 1 + get_random(size.x-2); break;
		case 1: offset.y = 1 + get_random(size.y-2); break;
		case 2: start.x = size.x-1; offset.y = 1 + get_random(size.y-2); break;
		case 3: start.y = size.y-1; offset.x = 1 + get_random(size.x-2); break;
    }

    point* res = malloc(sizeof(point));
    res->x = start.x + offset.x;
    res->y = start.y + offset.y;

    if (res->x != 0 && res->x != size.x-1) {
		res->x = res->x / 2 * 2 - 1;
		if (res->x < 1) 
			res->x = 1;
    }

    maze[res->y][res->x] = 'X';

    // offset from 'X'
    if (res->y == 0) res->y++;
    if (res->y == size.y-1) res->y--;
    if (res->x == 0) res->x++;
    if (res->x == size.x-1) res->x--;

    return res;
}

void show_progress(point current, char* wait) {
    clear();
    move(0, 0);

    for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++)
			if (current.x == x && current.y == y) addch('0' | COLOR_PAIR(2));
			else addch((chtype)maze[y][x]);
		addch('\n');
    }

    addch('\n');
	simple_offset("s : to skip");
    addch('s' | A_BOLD | COLOR_PAIR(1));
    printw(" : to skip");
    refresh();

    if (*wait && getch() == 's') *wait = 0;
}

void generate_maze(void) {
    fill_walls();

    // points in maze
    point in = {size.x-1, size.y-1};
	point* temp = set_goal();
    point current = *temp;
	free(temp);
   
    // history stack
    stack* stk = new_stack(in.x * in.y); 
    push(stk, current);

    timeout(5); char wait = 1; // fast showing progress
    while(stk->size > 0) {
		show_progress(current, &wait);

		maze[current.y][current.x] = ' ';

		// check for valid points
		int wall = 0;
		char valid[4] = {0};
		if (current.y + 2 < in.y && maze[current.y +2][current.x] == '#') 
			{ valid[wall] = 'S'; wall++;}
		if (current.y > 2 && maze[current.y -2][current.x] == '#') 
			{ valid[wall] = 'W'; wall++;}
		if (current.x + 2 < in.x && maze[current.y][current.x +2] == '#') 
			{ valid[wall] = 'D'; wall++;}
		if (current.x > 2 && maze[current.y][current.x -2] == '#') 
			{ valid[wall] = 'A'; wall++;}
		
		if (wall) {
			push(stk, current);

			switch(valid[get_random(wall)]) {
			case 'S': maze[current.y+1][current.x] = ' '; current.y += 2; break;
			case 'W': maze[current.y-1][current.x] = ' '; current.y -= 2; break;
			case 'D': maze[current.y][current.x+1] = ' '; current.x += 2; break;
			case 'A': maze[current.y][current.x-1] = ' '; current.x -= 2; break;
			}
		} else current = *pop(stk);
    }

    deallocate(stk);
    timeout(TIMEOUT); // return to normal speed
}

