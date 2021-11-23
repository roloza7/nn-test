#include "nodebatch.h"

class InputBatch : NodeBatch {
    public:
        InputBatch() = delete;
        InputBatch(size_t input_node_count);

        vector<double>* AddSeries(vector<double>* input);

        void Propagate(vector<double>& out);

};