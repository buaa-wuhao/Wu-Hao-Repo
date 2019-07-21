#ifndef SPARSEMATRIX_H_INCLUDED
#include "SparseMatrix.h"
#endif // SPARSEMATRIX_H_INCLUDED

#include <iostream>

using namespace std;

int main()
{
    SparseMatrix a(4,4);
    SparseMatrix b(4,4);

        a.insertElement(1, 2, 10);
        a.insertElement(1, 4, 12);
        a.insertElement(3, 3, 5);
        a.insertElement(4, 1, 15);
        a.insertElement(4, 2, 12);
        b.insertElement(1, 3, 8);
        b.insertElement(2, 4, 23);
        b.insertElement(3, 3, 9);
        b.insertElement(4, 1, 20);
        b.insertElement(4, 2, 25);

        //a.print_matrix();
        cout << "Hello world!" << endl;
        SparseMatrix c(4,4);
        c = a.multiply(b);
       // cout<<c.sparseMatrix.size()<<endl;
       // b.print_matrix();
       // a.transpose();
        cout << "Hello world!" << endl;
        c.print_matrix();
        //b.transpose();
        //b.print_matrix();
    return 0;
}
