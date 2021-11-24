#include "neuralnetwork.h"
#include <algorithm>

using std::vector;

NeuralNetwork::NeuralNetwork(unsigned size) : NeuralNetwork(size, Activators::FunctionType::BinaryStep) {};

NeuralNetwork::NeuralNetwork(unsigned size, Activators::FunctionType ftype) : initialised_(false), size_(size), input_nodes_(size), output_nodes_(size) {
    inner_nodes_ = new vector<double>(size_);
    working_buffer_ = new vector<double>(size_);
    inner_weights_ = new SparseMatrix<double>(size_, size_);
    inner_biases_ = new vector<double>(size_);

    std::pair<double (*)(double), double (*)(double)> activator_f_df = Activators::ActivatorMap.find(ftype)->second;
    activator_ = activator_f_df.first;
    activator_derivative_ = activator_f_df.second;

    Initialise();
}


void NeuralNetwork::Initialise() {
    for (size_t i = 0; i < size_; ++i) {
        (*inner_nodes_)[i] = 0.1;
        SparseMatrix<double>::RowProxy row = (*inner_weights_)[i];
        for (size_t j = 0; j < size_; ++j)
            row[j] = BASE_WEIGHTS;
        (*inner_biases_)[i] = 0;
    }
    initialised_ = true;
}

void NeuralNetwork::AddInput(vector<double>* input_nodes) {
    input_nodes_.AddSeries(input_nodes);
}

void NeuralNetwork::AddOutput(vector<double>* output_nodes) {
    output_nodes_.AddSeries(output_nodes);
}

void NeuralNetwork::Step() {
    std::fill(working_buffer_->begin(), working_buffer_->end(), 0);
    
    Dampen();

    input_nodes_.Propagate(*working_buffer_);

    Propagate();
    
    output_nodes_.Propagate(*inner_nodes_);
}


/**
 *  Steps inner nodes.
 *  Switches between the node storage and a working buffer.
 * */
void NeuralNetwork::Propagate() {
    LinAlg::Multiply(*inner_nodes_, *inner_weights_, *working_buffer_);
    vector<double>* t = inner_nodes_;
    inner_nodes_ = working_buffer_;
    working_buffer_ = t;

    for (unsigned i = 0; i < size_; i++) {
        (*inner_nodes_)[i] += (*inner_biases_)[i];
        (*inner_nodes_)[i] = activator_((*inner_nodes_)[i]);
    }
}

void NeuralNetwork::Dampen() {
    for (double& v : *inner_nodes_)
        v *= DAMPENING;
}

void NeuralNetwork::PrintInnerNodes(std::ostream& os) {
    os << "[ ";
    for (auto it = inner_nodes_->begin(); it != inner_nodes_->end(); ++it) {
        os << *it << " ";
    }
    os << "]" << std::endl;
    return;
}

void NeuralNetwork::PrintOutputNodes(std::ostream& os, int index) {
    const vector<vector<double>*> series = output_nodes_.GetSeries();


    os << "[ ";
    for (const double& d : *(series)[index])
        os << d << " ";
    os << "]\n";


    os << std::endl;

}