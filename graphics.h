#ifndef GRAPHICS
#define GRAPHICS

typedef enum field_type_def {SHORT, LONG} field_type;

void initialize_ncurses(void);
void print_maze(void);
void set_colors(void);
void clear_screen(void);

#endif
