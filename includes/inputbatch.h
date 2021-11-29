#pragma once
#include "nodebatch.h"

class InputBatch : public NodeBatch {
    public:
        InputBatch() = delete;
        InputBatch(size_t inner_node_count);

        vector<double>* AddSeries(vector<double>* series, vector<double>* capacity);

        void Propagate(vector<double>& out) const;

        vector<vector<double>*>& GetSeries();
        vector<vector<double>*>& GetCapacities();

    private:
        vector<vector<double>*> capacity_;

};