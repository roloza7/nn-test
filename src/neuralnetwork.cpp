// // #include "neuralnetwork.h"
// // #include <algorithm>

// // NeuralNetwork::NeuralNetwork(unsigned size) : NeuralNetwork(size, Activators::FunctionType::Logistic) {};

// // NeuralNetwork::NeuralNetwork(unsigned size, Activators::FunctionType ftype) : initialised_(false), size_(size), 
// //                             n_inputs_(0), n_outputs_(0) {
// //     inner_nodes_ = new std::vector<double>(size_);
// //     working_buffer_ = new std::vector<double>(size_);
// //     inner_weights_ = new SparseMatrix<double>(size_, size_);
// //     inner_biases_ = new std::vector<double>(size_);

// //     std::pair<double (*)(double), double (*)(double)> activator_f_df = Activators::ActivatorMap.find(ftype)->second;
// //     activator_ = activator_f_df.first;
// //     activator_derivative_ = activator_f_df.second;

// //     Initialise();
// // }


// void NeuralNetwork::Initialise() {
//     for (size_t i = 0; i < size_; ++i) {
//         (*inner_nodes_)[i] = 0.1;
//         for (size_t j = 0; j < size_; ++j)
//             (*inner_weights_)(i, j) = BASE_WEIGHTS;
//         (*inner_biases_)[i] = 0;
//     }
//     initialised_ = true;
// }

// void NeuralNetwork::AddInput(std::vector<double>* input_nodes) {
//     input_nodes_.push_back(input_nodes);
//     SparseMatrix<double>* sm = new SparseMatrix<double>(size_, input_nodes->size());
//     input_weights_.push_back(sm);
//     for (size_t i = 0; i < size_; ++i) {
//         for (size_t j = 0; j < input_nodes->size(); ++j) {
//             (*sm)(i, j) = BASE_WEIGHTS;
//         }
//     }
//     n_inputs_++;
// }

// void NeuralNetwork::AddOutput(std::vector<double>* output_nodes) {
//     output_nodes_.push_back(output_nodes);
//     SparseMatrix<double>* sm = new SparseMatrix<double>(output_nodes->size(), size_);
//     output_weights_.push_back(sm);
//     for (size_t i = 0; i < output_nodes->size(); ++i) {
//         for (size_t j = 0; j < size_; ++j) {
//             (*sm)(i, j) = BASE_WEIGHTS;
//         }
//     }
//     n_outputs_++;
// }

// void NeuralNetwork::Step() {
//     std::fill(working_buffer_->begin(), working_buffer_->end(), 0);
//     StepInputNodes();
//     StepInnerNodes();
//     Dampen();
//     StepOutputNodes();
// }

// void NeuralNetwork::StepInputNodes() {
//     for (size_t i = 0; i < n_inputs_; ++i) {
//         input_weights_[i]->Multiply(input_nodes_[i], inner_nodes_);
//     }
// }


// /**
//  *  Steps inner nodes.
//  *  Switches between the node storage and a working buffer.
//  * */
// void NeuralNetwork::StepInnerNodes() {
//     inner_weights_->Multiply(inner_nodes_, working_buffer_);
//     std::vector<double>* t = inner_nodes_;
//     inner_nodes_ = working_buffer_;
//     working_buffer_ = t;

//     for (unsigned i = 0; i < size_; i++) {
//         (*inner_nodes_)[i] += (*inner_biases_)[i];
//         (*inner_nodes_)[i] = activator_((*inner_nodes_)[i]);
//     }
// }

// void NeuralNetwork::StepOutputNodes() {
//     for (size_t i = 0; i < n_outputs_; ++i) {
//         output_weights_[i]->Multiply(inner_nodes_, output_nodes_[i]);
//     }
// }

// void NeuralNetwork::Dampen() {
//     for (double& v : *inner_nodes_)
//         v *= DAMPENING;
// }

// void NeuralNetwork::PrintInnerNodes(std::ostream& os) {
//     os << "[ ";
//     for (auto it = inner_nodes_->begin(); it != inner_nodes_->end(); ++it) {
//         os << *it << " ";
//     }
//     os << "]" << std::endl;
//     return;
// }

// void NeuralNetwork::PrintOutputNodes(std::ostream& os, int index) {
//     os << "[ ";
//     for (auto it = output_nodes_[index]->begin(); it != output_nodes_[index]->end(); ++it) {
//         os << *it << " ";
//     } 
//     os << "]" << std::endl;
//     return;
// }