#include "inputbatch.h"
#include "sparsematrix.h"

using std::vector;
using std::size_t;

InputBatch::InputBatch(size_t inner_node_count) : NodeBatch(inner_node_count) {};

vector<double>* InputBatch::AddSeries(vector<double>* series, vector<double>* capacity) {
    SparseMatrix<double>* sm = new SparseMatrix<double>(inner_node_count_, series->size());
    weights_.push_back(sm);
    nodes_.push_back(series);
    n_interfaces_++;
    capacity_.push_back(capacity);
    return series;
}

void InputBatch::Propagate(vector<double>& out) const {
    for (size_t i = 0; i < n_interfaces_; i++)
        LinAlg::Multiply(*nodes_[i], *weights_[i], out);
}

vector<vector<double>*>& InputBatch::GetSeries() {
    return nodes_;
}

vector<vector<double>*>& InputBatch::GetCapacities() {
    return capacity_;
}
