#include "graphics.h" // header

#include "types.h" // global variables
#include "messages.h" // controls
#include "sight.h"
#include <locale.h>
#include <ncurses.h>

char initialized = 0;

const char* C_HORIZONTAL = "─";
const char* C_HORIZONTAL_UP = "┴";
const char* C_HORIZONTAL_DOWN = "┬";
const char* C_CROSS = "┼";
const char* C_VERTICAL = "│";
const char* C_VERTICAL_LEFT = "┤";
const char* C_VERTICAL_RIGHT = "├";
const char* C_CORNER_DOWN_RIGHT = "┌";
const char* C_CORNER_DOWN_LEFT = "┐";
const char* C_CORNER_UP_RIGHT = "└";
const char* C_CORNER_UP_LEFT = "┘";

void initialize_ncurses(void) {
    if (!initialized) {
		setlocale(LC_ALL, "");

        initscr(); // initialize ncurses
        cbreak(); // no line buffering
        noecho(); // don't display characters
        curs_set(0); // hide cursor
        set_colors();
		timeout(TIMEOUT);
		
		initialized = 1;
    }
}

void print_troll(field_type type, int id) {
    attron(COLOR_PAIR(2));

    switch (type) {
        case SHORT: 
			if (troll_alive[id]) addch('T'); 
			else addch('~');
			break;
        case LONG:
			if (troll_alive[id]) printw(" T "); 
			else printw(" ~ ");
			break;
    }   

    attroff(COLOR_PAIR(2));
}

void print_player(field_type type) {
    attron(COLOR_PAIR(1));

    switch (type) {
        case SHORT:
            switch (direct) {
                case UP: addch('^'); break;
                case DOWN: addch('v'); break;
                case LEFT: addch('<'); break;
                case RIGHT: addch('>'); break;
            }   
			break;
        case LONG:
            switch (direct) {
                case UP: printw(" ^ "); break;
                case DOWN: printw(" v "); break;
                case LEFT: printw(" < "); break;
                case RIGHT: printw(" > "); break;
            }   
			break;
    }   

    attroff(COLOR_PAIR(1));
}

void print_portal(field_type type) {

    attron(COLOR_PAIR(3));

    switch (type){
        case SHORT: addch('O'); break;
        case LONG: printw(" O "); break;
    }

    attroff(COLOR_PAIR(3));
}

void print_space(field_type type, point place) {
	int c = maze[place.y][place.x];

    if (c == '\0') {
		addch('\n');
		return;
    }

	if (c == 'X') {
		if (type == LONG) {
			attron(COLOR_PAIR(1));
			printw(" X ");
			attroff(COLOR_PAIR(1));

		} else addch('X' | COLOR_PAIR(1));

		return;
	}

	if (c == ' ') {
		if (type == SHORT) addch(' ');
		else printw("   ");

		return;
	}

	char up = 0;
	char down = 0;
	char left = 0;
	char right = 0;

	if (place.y < size.y-1 && maze[place.y+1][place.x] == '#') down = 1;
	if (place.y > 0 && maze[place.y-1][place.x] == '#') up = 1;
	if (place.x < size.x-1 && maze[place.y][place.x+1] == '#') right = 1;
	if (place.x > 0 && maze[place.y][place.x-1] == '#') left = 1;

    switch (type){
        case SHORT:
			if (up && down && left && right) {
				printw(C_CROSS);
				return;
			}

			if (up && down) {
				if (left) {printw(C_VERTICAL_LEFT); return;}
				if (right) {printw(C_VERTICAL_RIGHT); return;}
				printw(C_VERTICAL); return;	
			}

			if (left && right) {
				if (up) {printw(C_HORIZONTAL_UP); return;}
				if (down) {printw(C_HORIZONTAL_DOWN); return;}
				printw(C_HORIZONTAL); return;
			}

			if (up) {
				if (left) {printw(C_CORNER_UP_LEFT); return;}
				if (right) {printw(C_CORNER_UP_RIGHT); return;}
			}

			if (down) {
				if (left) {printw(C_CORNER_DOWN_LEFT); return;}
				if (right) {printw(C_CORNER_DOWN_RIGHT); return;}
			}

			if (up || down) {
				printw(C_VERTICAL); return;
			}

			if (left || right) {
				printw(C_HORIZONTAL); return;
			}

			addch('+'); return;	
        case LONG:
			if (up && down && left && right) {
				printw("%s%s%s", C_HORIZONTAL , C_CROSS, C_HORIZONTAL);
				return;
			}

			if (up && down) {
				if (left) 
					{ printw("%s%s ", C_HORIZONTAL, C_VERTICAL_LEFT); return;}
				if (right) 
					{ printw(" %s%s", C_HORIZONTAL, C_VERTICAL_RIGHT); return;}
				printw(" %s ", C_VERTICAL); return;	
			}

			if (left && right) {
				if (up) { printw("%s%s%s", C_HORIZONTAL, C_HORIZONTAL_UP, C_HORIZONTAL); return;}
				if (down) { printw("%s%s%s", C_HORIZONTAL, C_HORIZONTAL_DOWN, C_HORIZONTAL); return;}
				printw("%s%s%s", C_HORIZONTAL, C_HORIZONTAL, C_HORIZONTAL); return;
			}

			if (up) {
				if (left) {printw("%s%s ", C_HORIZONTAL, C_CORNER_UP_LEFT); return;}
				if (right) {printw(" %s%s", C_CORNER_UP_RIGHT, C_HORIZONTAL); return;}
			}

			if (down) {
				if (left) {printw("%s%s ", C_HORIZONTAL, C_CORNER_DOWN_LEFT); return;}
				if (right) {printw(" %s%s", C_CORNER_DOWN_RIGHT, C_HORIZONTAL); return;}
			}
			

			if (up || down) {
				printw(" %s ", C_VERTICAL); return;
			}

			if (left || right) {
				printw("%s%s%s", C_HORIZONTAL, C_HORIZONTAL, C_HORIZONTAL); return;
			}

			printw(" + "); return;
    }
}

void print_void(field_type type) {
	if (type == SHORT) addch('?');
	else printw("???");
}

void print_field(field_type type, point place) {
	if (maze[place.y][place.x] == '\0') {
		addch('\n');
		return;
	}

	if (!is_visible(&place)) {
		print_void(type);
		return;
	}

    for (int i = 0; i < portals*2; i++) {
		if (place.x == portal[i].x && place.y == portal[i].y) {
			print_portal(type);
			return;
		}
    }

    if (place.x == player.x && place.y == player.y) {
        print_player(type);
		return;
    }

    char troll_found = 0;
    int troll_id;
    for (int i = 0; i < trolls; i++) {
		if (troll[i].x == place.x && troll[i].y == place.y) {
			troll_id = i;
			troll_found = 1;
			break;
		}
    }

    if (troll_found && maze[place.y][place.x] != '#') print_troll(type, troll_id);
    else print_space(type, place);
}

void print_maze(void) {
    clear_screen();
	process_sight();

	attron(A_BOLD);

    field_type type;
    for (int y = 0; y < size.y; y ++) 
	// !IMPORTANT
	// must have 1 extra space for \0
        for (int x = 0; x <= size.x; x ++) {
            if (x % 2 == 0)
                type = SHORT;
            else
                type = LONG;
	    
	    point place = {x,y};
        print_field(type, place);
        }

	attroff(A_BOLD);

    controls_message();
    refresh(); // PRINT IT ! :D 
}

void clear_screen(void) {
    clear();
    move(0, 0);
}

void set_colors(void) {
    start_color();

    init_color(7, 1000, 1000, 1000);
    init_color(15, 1000, 1000, 1000);
    init_color(0, 89, 93, 101);
    //init_color(4, 871, 738, 214);

    init_pair(1, 0, 0);
    init_pair(2, 6, 0);
    init_pair(3, 9, 0);
    init_pair(4, 14, 0);
}

