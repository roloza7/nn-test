#ifndef LINALG_H_
#define LINALG_H_

#include <vector>

using std::vector;

template<typename T> class SparseMatrix;

namespace LinAlg {

template<typename T>
vector<T>   Multiply(const vector<T>& left, const SparseMatrix<T>& right);

template<typename T>
void        Multiply(const vector<T>& left, const SparseMatrix<T>& right, vector<T>& out, bool clear = false);

};

#include "../src/linalg.ipp"

#endif // LINALG_H_