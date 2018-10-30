#include <stdlib.h>
#include <stdbool.h>

static int N;
static int* stack;
static int maxLimit;
void stackInit(int maxN)
{
    stack = malloc(sizeof(int)*maxN);
    N = 0;
    maxLimit = maxN;
}

bool isStackFull()
{
    return N==maxLimit;
}

bool isStackEmpty()
{
    return N==0;
}

void stackPush(int x)
{
    stack[N++] = x;
}

int stackPop()
{
    int rtn = stack[N--];
    return rtn;
}
