#ifndef SPARSEMATRIX_H_INCLUDED
#include "SparseMatrix.h"
#endif // SPARSEMATRIX_H_INCLUDED
#include <algorithm>
#include <iostream>
#include <climits>
using namespace std;

bool compTuple(tuple<int,int,int>&lhs,tuple<int,int,int>&rhs)
{
    if(get<0>(lhs)!=get<0>(rhs))
    {
        return get<0>(lhs) < get<0>(rhs);
    }

    return get<1>(lhs) < get<1>(rhs);
}



SparseMatrix SparseMatrix::multiply(SparseMatrix B)
{
    SparseMatrix res(rows,B.cols);
    if(cols!=B.rows) return res;
    B.transpose();
    B.print_matrix();
    int idx_a = 0;
    int sz_a  = sparseMatrix.size();
    int sz_b  = B.sparseMatrix.size();

    for(;idx_a<sz_a;idx_a++)
    {
        int row = get<0>(sparseMatrix[idx_a]);
        for(int idx_b = 0;idx_b<sz_b;idx_b++)
        {
            int col = get<0>(B.sparseMatrix[idx_b]);

            if(!res.sparseMatrix.empty())
            {
                int r = get<0>(res.sparseMatrix.back());
                int c = get<1>(res.sparseMatrix.back());
                if(row < r) continue;
                if(row==r && col<=c) continue;
            }


            int ii_a = idx_a,ii_b = idx_b;

            int sum = 0;
            while(ii_a < sz_a && row==get<0>(sparseMatrix[ii_a]) &&
                  ii_b < sz_b && col==get<0>(B.sparseMatrix[ii_b]))
            {
                if(get<1>(sparseMatrix[ii_a]) < get<1>(B.sparseMatrix[ii_b]) )
                {
                    ii_a++;
                }
                else if(get<1>(sparseMatrix[ii_a]) > get<1>(B.sparseMatrix[ii_b]) )
                {
                    ii_b++;
                }
                else
                {
                    sum += get<2>(sparseMatrix[ii_a]) * get<2>(B.sparseMatrix[ii_b]);
                    ii_a++;
                    ii_b++;
                }
            }
            if(sum!=0)
            res.sparseMatrix.push_back(make_tuple(row,col,sum));
        }
    }
    return res;

}



SparseMatrix SparseMatrix::add(SparseMatrix& B)
{

    SparseMatrix res(rows,cols);
    if(rows!=B.rows || cols!=B.cols)
    {
        cout << rows << " " << B.rows <<" " << cols << " "<<B.cols<<endl;
        cout<<"Matrix Size not Match!"<<endl;
        return res;
    }

    int idx_a=0, idx_b=0;
    size_t sz_a = sparseMatrix.size();
    size_t sz_b = B.sparseMatrix.size();

    while(idx_a<sz_a && idx_b<sz_b)
    {
        int row_a = get<0>(sparseMatrix[idx_a]);
        int col_a = get<1>(sparseMatrix[idx_a]);
        int val_a = get<2>(sparseMatrix[idx_a]);
        int row_b = get<0>(B.sparseMatrix[idx_b]);
        int col_b = get<1>(B.sparseMatrix[idx_b]);
        int val_b = get<2>(B.sparseMatrix[idx_b]);

        if(row_a==row_b && col_a==col_b)
        {
            res.sparseMatrix.push_back(make_tuple(row_a,col_a,val_a+val_b));
            idx_a++;
            idx_b++;
        }
        else if(row_a < row_b || (row_a==row_b && col_a<col_b))
        {
            res.sparseMatrix.push_back(sparseMatrix[idx_a]);
            idx_a++;
        }
        else
        {
            res.sparseMatrix.push_back(B.sparseMatrix[idx_b]);
            idx_b++;
        }
    }

    while(idx_a<sz_a)
    {
        res.sparseMatrix.push_back(sparseMatrix[idx_a]);
        idx_a++;
    }

    while(idx_b<sz_b)
    {
        res.sparseMatrix.push_back(sparseMatrix[idx_b]);
        idx_b++;
    }
    return res;
}

void SparseMatrix::transpose()
{
    for(int i=0;i<sparseMatrix.size();i++)
    {
        int tmp = get<0>(sparseMatrix[i]);
        get<0>(sparseMatrix[i]) = get<1>(sparseMatrix[i]);
        get<1>(sparseMatrix[i]) = tmp;
    }
    sortElement();
    return;
}

void SparseMatrix::print_matrix()
{
    for(int i=0;i<sparseMatrix.size();i++)
    {
        int r = get<0>(sparseMatrix[i]);
        int c = get<1>(sparseMatrix[i]);
        int v = get<2>(sparseMatrix[i]);
        cout<< r <<" " << c << " " << v << endl;
    }
}

SparseMatrix::SparseMatrix(int r,int c)
{
    rows = r;
    cols = c;
}

void SparseMatrix::insertElement(int r,int c,int val)
{
    sparseMatrix.push_back(make_tuple(r,c,val));
}

void SparseMatrix::sortElement()
{
    sort(sparseMatrix.begin(),sparseMatrix.end(),compTuple);
}
