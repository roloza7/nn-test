#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork(unsigned size) : NeuralNetwork(size, Activators::FunctionType::Logistic) {};

NeuralNetwork::NeuralNetwork(unsigned size, Activators::FunctionType ftype) : initialised_(false), size_(size) {
    nodes_ = new double[size_];
    working_buffer_ = new double[size_];
    weights_ = new double*[size_];
    biases_ = new double[size_];

    std::pair<double (*)(double), double (*)(double)> activator_f_df = Activators::ActivatorMap.find(ftype)->second;
    activator_ = activator_f_df.first;
    activator_derivative_ = activator_f_df.second;
}


void NeuralNetwork::Initialise() {
    for (size_t i = 0; i < size_; ++i) {
        nodes_[i] = 0;
        weights_[i] = new double[size_];
        for (size_t j = 0; j < size_; ++j)
            weights_[i][j] = BASE_WEIGHTS;
        biases_[i] = 0;
    }
    initialised_ = true;
}

void Step() {
    
}