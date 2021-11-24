#pragma once
#include "nodebatch.h"

using std::vector;
using std::size_t;

class OutputBatch : public NodeBatch {
    public:
        OutputBatch() = delete;
        OutputBatch(size_t inner_node_count);

        vector<double>* AddSeries(vector<double>* series);

        void Propagate(vector<double>& node_values);

        const vector<vector<double>*>& GetSeries() const;

};