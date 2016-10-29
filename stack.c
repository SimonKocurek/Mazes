#include "stack.h"
#include <stdlib.h>

stack* new_stack(int max) {
    stack* stk = malloc(sizeof(stack));
    stk->size = 0;
    stk->data = malloc((size_t)max * sizeof(point));
    return stk;
}

void push(stack* stk, point x) {
    stk->data[stk->size] = x;
    stk->size++;
}

point* pop(stack* stk) {
    stk->size--;
    return stk->data + stk->size;
}

void deallocate(stack* stk) {
    free(stk->data);
    free(stk);
}

