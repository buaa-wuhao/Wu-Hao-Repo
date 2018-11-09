#ifndef MERGE_H_INCLUDED
#include "merge.h"
#endif // MERGE_H_INCLUDED

static int* aux;

void mergeInit(int sz)
{
    aux = malloc(sz*sizeof(int));
}
/*
void Merge(int a[],int l,int m,int r)
{
    int i ,j;
    for(i=m+1;i>l;i--) aux[i-1] = a[i-1];
    for(j=m;j<r;j++)   aux[r+m-j] = a[j+1];
    for(int k=l;k<=r;k++)
    {
        if(aux[i]<aux[j])
        {
            a[k] = aux[i++];
        }
        else
        {
            a[k] = aux[j--];
        }
    }

    return;
}

void mergeSort(int a[],int l,int r)
{
    if(l>=r) return;
    int m = l + (r-l)/2;
    mergeSort(a,l,m);
    mergeSort(a,m+1,r);
    Merge(a,l,m,r);
    return;
}*/
