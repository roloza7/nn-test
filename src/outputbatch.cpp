#include "outputbatch.h"
#include "sparsematrix.h"

using std::vector;
using std::size_t;

OutputBatch::OutputBatch(size_t inner_node_count) : NodeBatch(inner_node_count) {};

vector<double>* OutputBatch::AddSeries(vector<double>* series, vector<double>* capacity) {
    SparseMatrix<double>* sm = new SparseMatrix<double>(series->size(), inner_node_count_);
    nodes_.push_back(series);
    weights_.push_back(sm);
    n_interfaces_++;
    capacity_.push_back(capacity);
    return series;
}

void OutputBatch::Propagate(vector<double>& values) {
    for (size_t i = 0; i < n_interfaces_; ++i)
        LinAlg::Multiply(values, *weights_[i], *nodes_[i], true);
}

const vector<vector<double>*>& OutputBatch::GetSeries() const {
    return nodes_;
}

vector<SparseMatrix<double>*>& OutputBatch::GetWeights() {
    return weights_;
}

vector<vector<double>*>& OutputBatch::GetCapacities() {
    return capacity_;
}