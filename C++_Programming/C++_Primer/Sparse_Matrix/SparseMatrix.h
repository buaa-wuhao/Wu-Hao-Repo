#ifndef SPARSEMATRIX_H_INCLUDED
#define SPARSEMATRIX_H_INCLUDED
#include <tuple>
#include <vector>
class SparseMatrix{
    private:
        int rows;
        int cols;
    public:
        std::vector<std::tuple<int,int,int>> sparseMatrix;
        SparseMatrix(int r,int c);
        SparseMatrix add(SparseMatrix& B);
        void insertElement(int r,int c,int val);
        void sortElement();
        void transpose();
        SparseMatrix multiply(SparseMatrix B);
        void print_matrix();
};




#endif // SPARSEMATRIX_H_INCLUDED
