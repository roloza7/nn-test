#pragma once

#ifndef INPUT_BATCH_H_
#define INPUT_BATCH_H_

#include <cstddef>
#include <vector>
#include "linalg.h"

using std::size_t;
using std::vector;

/**
 * @brief Stores a series of input node value/weight information.
 * 
 * 
 */
class NodeBatch {
    public:
        NodeBatch() = delete;
        NodeBatch(size_t inner_node_count);

        virtual vector<double>* AddSeries(vector<double>* series) = 0;

        const vector<SparseMatrix<double>*>& GetWeights() const;
        vector<SparseMatrix<double>*>& GetWeights();

        const SparseMatrix<double>& GetWeight(int index) const;
        SparseMatrix<double>& GetWeight(int index);

        size_t Size() const;

    protected:
        size_t n_interfaces_;
        size_t inner_node_count_;
        vector<vector<double>*> nodes_;
        vector<SparseMatrix<double>*> weights_;

};

#endif // INPUT_BATCH_H_