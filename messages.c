#include "messages.h" // header

#include "types.h" // global variables
#include <ncurses.h>
#include <string.h> // strlen

void simple_offset(const char* message) {
	int offset = (int)strlen(message);
	int start = size.x / 2 - offset / 2;
	for (int i = 0; i < start; i++)
		addch(' ');
}

void offset_by_whitespace(const char* message) {
    // average character has length of 2 -> size.x * 2 / 2
    int field_half = size.x;
    int offset = (int)strlen(message);

    int start = field_half - offset /2;
    for (int i = 0; i < start; i++)
	    addch(' ');
}

void controls_message(void) {
    addch('\n');
    offset_by_whitespace("q : quit | r : reset | wsad : movement");
    
    addch('q' | A_BOLD | COLOR_PAIR(1)); printw("%s", " : quit | ");
    addch('r' | A_BOLD | COLOR_PAIR(1)); printw("%s", " : reset | ");
    addch('w' | A_BOLD | COLOR_PAIR(1));
    addch('s' | A_BOLD | COLOR_PAIR(1));
    addch('a' | A_BOLD | COLOR_PAIR(1));
    addch('d' | A_BOLD | COLOR_PAIR(1)); printw("%s", " : movement");
}

void win_message(void) {
    clear();
    move(0, 0);

    // offset by height
    int half = size.y / 2 - 3;
    for (int i = 0; i < half; i++)
		addch('\n');

    attron(A_BOLD | COLOR_PAIR(1));
    
    offset_by_whitespace("You Won!");    
    printw("You Won!");

    attroff(A_BOLD | COLOR_PAIR(1));
}

void lost_message(void) { 
	clear();
    move(0, 0);

    // offset by height
    int half = size.y / 2 - (size.y % 2);
    for (int i = 0; i < half-2; i++)
		addch('\n');

    attron(A_BOLD | COLOR_PAIR(2));

    offset_by_whitespace("You Lost :(");
    printw("You Lost :(");

    attroff(A_BOLD | COLOR_PAIR(2));
}

void score_message(void) {
	printw("\n\n");

	offset_by_whitespace("wins : 0 loses : 0");
	printw("wins: ");
	attron(A_BOLD | COLOR_PAIR(1)); printw("%d", wins); attroff(A_BOLD | COLOR_PAIR(1));
	printw(" loses : ");
	attron(A_BOLD | COLOR_PAIR(2)); printw("%d", loses); attroff(A_BOLD | COLOR_PAIR(2));
}

int reset_message(void) {
	score_message();

	printw("\n\n");

    offset_by_whitespace("Do you want to reset? [y/n]");
    printw("Do you want to reset? ["); addch('y' | A_BOLD | COLOR_PAIR(1));
	addch('/'); addch('n' | A_BOLD | COLOR_PAIR(2)); addch(']');

	timeout(-1);
	int result = getch();
	timeout(TIMEOUT);
    return result;
}

