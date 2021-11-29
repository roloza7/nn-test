#pragma once
#include "nodebatch.h"

using std::vector;
using std::size_t;

class OutputBatch : public NodeBatch {
    public:
        OutputBatch() = delete;
        OutputBatch(size_t inner_node_count);

        vector<double>* AddSeries(vector<double>* series, vector<double>* capacity);

        void Propagate(vector<double>& node_values);

        const vector<vector<double>*>& GetSeries() const;
        vector<SparseMatrix<double>*>& GetWeights();
        vector<vector<double>*>& GetCapacities();

    private:
        vector<vector<double>*> capacity_;

};