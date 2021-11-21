#ifndef SPARSE_MATRIX_H_
#define SPARSE_MATRIX_H_

#include <vector>
#include <map>
#include <cstddef>
#include <ostream>

using std::vector;
using std::size_t;
using std::ostream;
using std::map;

template<typename T> class SparseMatrix;
template<typename T> ostream& operator<<(ostream& os, const SparseMatrix<T>& sm); 

template<typename T>
class SparseMatrix {
    public:
        SparseMatrix();
        SparseMatrix(int width, int height);
        T& operator()(int x, int y);

        friend ostream& operator<< <> (ostream& os, const SparseMatrix<T>& sm);

        void Multiply(std::vector<T>*& values, std::vector<T>*& out) const;
        

    private:
        size_t width_;
        size_t height_;
        vector<std::map<size_t, T>> data_;

};

#include "../src/sparsematrix.ipp"

#endif // SPARSE_MATRIX_H_