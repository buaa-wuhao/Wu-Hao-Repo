#include <stdio.h>
#include <stdlib.h>
#ifndef STACK_H_INCLUDED
#include "stack.h"
#endif // STACK_H_INCLUDED

#define exch(A,B) {int t = A; A = B; B=t;}
void swap(int* x,int* y)
{
    int tmp = *x;
    *x      = *y;
    *y      = tmp;
    return;
}

int partition(int a[],int l,int r)
{
    int i = l-1,j=r;
    int v = a[r];
    while(1)
    {
        while(a[++i] < v);
        while(a[--j] > v) if(j==l) break;
        if(i>=j) break;
        swap(&a[i],&a[j]);
    }
    swap(&a[r],&a[i]);
    return i;
}
/*
void quickSort(int a[],int l,int r)
{
    if(l>=r) return;
    int i = partition(a,l,r);
    quickSort(a,l,i-1);
    quickSort(a,i+1,r);
    return;
}*/

void quickSort(int a[],int l,int r)
{
    int sz = r - l + 1;
    stackInit(sz);
    stackPush(r);
    stackPush(l);

    while(!isStackEmpty())
    {//printf("Hello world!\n");
        int right = stackPop();
        int left  = stackPop();
        printf("\n (%d,%d) \n",left,right);
        if(left>=right) continue;
        int i = partition(a,left,right);

        if(i-left > right-i)
        {
            stackPush(left);stackPush(i-1);
            stackPush(i+1); stackPush(right);
        }
        else
        {
            stackPush(i+1); stackPush(right);
            stackPush(left);stackPush(i-1);
        }
    }
    return;
}

#define N 19
int main()
{

    int a[N];
    for(int i=0;i<N;i++)
    {
        a[i] = rand()%100;
       // printf("%d, ",a[i]);
    }

    quickSort(a,0,N-1);

    for(int i=0;i<N;i++)
    {
        printf("%d, ",a[i]);
    }

    return 0;
}
