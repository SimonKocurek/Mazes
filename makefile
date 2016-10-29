FILES=sight.c graphics.c movement.c mazes.c spawner.c messages.c stack.c main.c
FLAGS=-m64 -std=c99 -O3
DEBUGFLAGS=-g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wconversion -Wunreachable-code

OUTPUT=-o mazing

all: main

main:
	gcc $(FLAGS) $(FILES) $(OUTPUT) -lncursesw

