#ifndef STACK
#define STACK

#include "types.h" // point

typedef struct stack_def {
    point* data;
    int size;
} stack;

stack* new_stack(int max);
void push(stack* stk, point x);
point* pop(stack* stk);
void deallocate(stack* stk);

#endif
