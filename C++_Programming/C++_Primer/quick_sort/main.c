#include <stdio.h>
#include <stdlib.h>
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
    int v = a[r];
    int i = l-1,j = r;
    for(;;)
    {
        while(a[++i] < v);
        while(a[--j] > v) if(j==l) break;
        if(i>=j) break;
        swap(&a[i],&a[j]);
    }
    swap(&a[i],&a[r]);
    return i;
}

void quickSort(int a[],int l,int r)
{
    printf("l:%d,r:%d \n",l,r);
    if(l>=r) return;
    int idx = partition(a,l,r);
    quickSort(a,l,idx-1);
    quickSort(a,idx+1,r);
    return;
}
#define N 19
int main()
{

    int a[N];
    for(int i=0;i<N;i++)
    {
        a[i] = rand()%100;
        printf("%d, ",a[i]);
    }
    printf("Hello world!\n");
    quickSort(a,0,N-1);

    for(int i=0;i<N;i++)
    {
        printf("%d, ",a[i]);
    }

    return 0;
}
