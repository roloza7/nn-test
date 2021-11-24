#include "nodebatch.h"
#include "sparsematrix.h"
#include <cstddef>

using std::vector;
using std::size_t;

NodeBatch::NodeBatch(size_t node_count) : n_interfaces_(0), inner_node_count_(node_count) {};
const vector<SparseMatrix<double>*>& NodeBatch::GetWeights() const { return weights_; }
vector<SparseMatrix<double>*>& NodeBatch::GetWeights() { return weights_; }

const SparseMatrix<double>& NodeBatch::GetWeight(int index) const {
    if (index < 0 || (size_t) index >= weights_.size())
        throw std::out_of_range("Weight index out of range");
    
    return *weights_[index];
}
SparseMatrix<double>& NodeBatch::GetWeight(int index) {
    if (index < 0 || (size_t) index >= weights_.size())
        throw std::out_of_range("Weight index out of range");
    
    return *weights_[index];
}

size_t NodeBatch::Size() const {
    return n_interfaces_;
}