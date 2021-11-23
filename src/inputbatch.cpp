#include "inputbatch.h"
#include "sparsematrix.h"

using std::vector;

InputBatch::InputBatch(size_t input_node_count) : NodeBatch(input_node_count) {};

vector<double>* InputBatch::AddSeries(vector<double>* input) {
    SparseMatrix<double>* sm = new SparseMatrix<double>(node_count_, input->size());
}
