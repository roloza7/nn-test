#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork(unsigned size) : NeuralNetwork(size, Activators::FunctionType::Logistic) {};

NeuralNetwork::NeuralNetwork(unsigned size, Activators::FunctionType ftype) : initialised_(false), size_(size) {
    inner_nodes_ = new std::vector<double>(size_);
    working_buffer_ = new std::vector<double>(size_);
    inner_weights_ = new SparseMatrix<double>(size_, size_);
    inner_biases_ = new std::vector<double>(size_);

    std::pair<double (*)(double), double (*)(double)> activator_f_df = Activators::ActivatorMap.find(ftype)->second;
    activator_ = activator_f_df.first;
    activator_derivative_ = activator_f_df.second;

    Initialise();
}


void NeuralNetwork::Initialise() {
    for (size_t i = 0; i < size_; ++i) {
        (*inner_nodes_)[i] = 0.1;
        for (size_t j = 0; j < size_; ++j)
            (*inner_weights_)(i, j) = BASE_WEIGHTS;
        (*inner_biases_)[i] = 0;
    }
    initialised_ = true;
}

void NeuralNetwork::Step() {
    inner_weights_->Multiply(inner_nodes_, working_buffer_);
    std::vector<double>* t = inner_nodes_;
    inner_nodes_ = working_buffer_;
    working_buffer_ = t;

    for (unsigned i = 0; i < size_; i++) {
        (*inner_nodes_)[i] += (*inner_biases_)[i];
        (*inner_nodes_)[i] = activator_((*inner_nodes_)[i]);
    }



    return;
}

void NeuralNetwork::StepInputNodes() {

}


/**
 *  Steps inner nodes.
 *  Switches between the node storage and a working buffer.
 * */
void NeuralNetwork::StepInnerNodes() {

    
}

void NeuralNetwork::StepOutputNodes() {
    
}

void NeuralNetwork::PrintInnerNodes(std::ostream& os) {
    os << "[ ";
    for (auto it = inner_nodes_->begin(); it != inner_nodes_->end(); ++it) {
        os << *it << " ";
    }
    os << "]" << std::endl;
    return;
}