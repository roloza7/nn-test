#pragma once

#ifndef TrainerWrapper
#define TrainerWrapper TrainerWrapper_impl
#endif

#include "neuralnetwork.h"
#include <cstddef>

using std::size_t;
using std::vector;

class TrainerWrapper {
public:
    TrainerWrapper() = delete;
    TrainerWrapper(int n_subjects, int network_size);
    ~TrainerWrapper();
    double** AddInputArray(int input_size);
    double** AddOutputArray(int output_size);
    void Step();
    void NextGeneration(int winner_id, int runner_up_id);
    void PrintInnerNodes();
    void PrintInputWeights();
    void PrintInputNodes();
    void PrintInputCapacities();
    void PrintOutputNodes();
private:
    void CreateRandomChildren();
    void InitializeRandomChildren();
    vector<vector<vector<double>*>> inputs_;
    vector<vector<vector<double>*>> outputs_;
    vector<NeuralNetwork*> networks_;
    NetworkStructure* recombination_buffer_;
    size_t n_subjects_;
    size_t n_inputs_;
    size_t n_outputs_;
    size_t network_size_;




    vector<NetworkStructure*>* networks_starting_info_;

};