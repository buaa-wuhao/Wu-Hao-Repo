#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <stdbool.h>
void stackInit(int maxN);
bool isStackFull();
bool isStackEmpty();
void stackPush(int x);
int  stackPop();

#endif // STACK_H_INCLUDED
