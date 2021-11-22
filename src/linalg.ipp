#include "linalg.h"
#include "except.hpp"

namespace LinAlg {

template<typename T>
vector<T>   Multiply(const vector<T>& left, const SparseMatrix<T>& right) {
    vector<T> out(right.width_, T());
    Multiply(left, right, out);
    return out;
}

template<typename T>
void        Multiply(const vector<T>& left, const SparseMatrix<T>& right, vector<T>& out, bool clear) {

    // Assure we are multiplying (1 x N) * (N x M)
    if (left.size() != right.height_)
        throw new Except::LinearAlgebraException();

    if (clear)
        std::fill(out.begin(), out.end(), T());

    if (out.size() != right.width_)
        out.resize(right.width_, T());

    for (unsigned y = 0; y < left.size(); ++y) {
        typename SparseMatrix<T>::RowProxy row = right[y];
        for (unsigned x = 0; x < right.width_; ++x) {
            out[x] += left[y] * row[x];
        }
    }

}



}