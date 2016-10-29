#include <ncurses.h>

#include "types.h" // global variables and types
#include "mazes.h" // reading/ generating mazes
#include "graphics.h"
#include "movement.h"
#include "spawner.h" // random generation and spawning monsters
#include "messages.h" // end game screens
#include "sight.h"

// maze
point size;
int** maze;

// player
point player;
direction direct = UP;

// trolls
int trolls;
point* troll;
char* troll_alive;

// portals
int portals;
point* portal;
point* portal_exit;

// score
int wins = 0;
int loses = 0;

int game_reset(void);

int main(void) {
    initialize_ncurses();
    initialize_rand(); // set seed for random
    initialize_maze(); // generate lenghts and fill

    generate_maze();
    place_player();
	int boost = (wins - loses) / 2;
	if (boost < -2) boost = -2; 
    place_trolls((size.x + size.y) / 15 + boost);
	if (boost > 2) boost = 2;
    place_portals((size.x + size.y) / 15 - boost);
    print_maze();
    
    // game loop
    int key = getch();
    while (key != 'q') {
		if (key == 'r') {
			loses++;
			return game_reset();
		}

		// player turn
		change_direction(key);
		move_player(key);

		// win
		if (maze[player.y][player.x] == 'X') {
			wins++;
			win_message();
			if (reset_message() == 'y') return game_reset();
			
			if (getch() == 'y') return game_reset();
			else break;
		}

		// troll turn
		for (int i = 0; i < trolls; i++) {
			if (!troll_alive[i])
				continue;

			move_troll(i);

			if (troll[i].x != player.x || troll[i].y != player.y)
				continue;

			loses++;
			lost_message();

			if (reset_message() == 'y')
				return game_reset();
			
			if (getch() == 'y')
				return game_reset();

			clear_screen();
			endwin(); 
			return 0;
		}

		// portal turn
		portal_teleport();

		print_maze();
		key = getch();
	}
   
	clear_screen();
	endwin(); // end ncurses
    return 0;
}

int game_reset(void) {
    free_maze();
    free_spawns();
	free_sight();
    return main();
}

