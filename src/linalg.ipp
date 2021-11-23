#include "linalg.h"
#include "except.hpp"
#include <map>
namespace LinAlg {

template<typename T>
vector<T>   Multiply(const vector<T>& left, const SparseMatrix<T>& right) {
    vector<T> out(right.width_, T());
    Multiply(left, right, out);
    return out;
}

template<typename T>
void        Multiply(const vector<T>& left, const SparseMatrix<T>& right, vector<T>& out, bool clear) {

    // Assure we are multiplying (1 x N) * (N x M) = (1 x M)
    if (left.size() != right.height_)
        throw new Except::LinearAlgebraException();

    if (clear)
        std::fill(out.begin(), out.end(), T());

    if (out.size() != right.width_)
        out.resize(right.width_, T());

    for (unsigned y = 0; y < left.size(); ++y) {
        typename std::map<size_t, T>::iterator it = right.data_[y]->begin();
        for (; it != right.data_[y]->end(); ++it)
            out[it->first] += left[y] * it->second;

    }

}



}