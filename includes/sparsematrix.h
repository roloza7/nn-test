#ifndef SPARSE_MATRIX_H_
#define SPARSE_MATRIX_H_

#include <vector>
#include <map>
#include <cstddef>
#include <ostream>
#include "linalg.h"

using std::vector;
using std::size_t;
using std::ostream;
using std::map;

template<typename T> class SparseMatrix;
template<typename T> ostream& operator<<(ostream& os, const SparseMatrix<T>& sm); 
template<typename T>
class SparseMatrix {
    public:

        class RowProxy {
            public:
                RowProxy(std::map<size_t, T>* row, size_t width);
                T& operator[](int index);
            private:
                size_t width_;
                std::map<size_t, T>* row_;
        };

        SparseMatrix();
        SparseMatrix(int width, int height);
        SparseMatrix(size_t width, size_t height);
        RowProxy operator[](int index);
        const RowProxy operator[](int index) const;    

        // Friends
        friend ostream& operator<< <> (ostream& os, const SparseMatrix<T>& sm);    
        friend vector<T> LinAlg::Multiply <> (const vector<T>& left, const SparseMatrix<T>& right);
        friend void LinAlg::Multiply <> (const vector<T>& left, const SparseMatrix<T>& right, vector<T>& out, bool clear);

    private:
        size_t width_;
        size_t height_;
        vector<std::map<size_t, T>*> data_;

};

#include "../src/sparsematrix.ipp"

#endif // SPARSE_MATRIX_H_