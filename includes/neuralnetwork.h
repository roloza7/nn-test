// #ifndef NEURAL_NETWORK_H_
// #define NEURAL_NETWORK_H_

// #include "activators.h"
// #include "sparsematrix.h"

// #include <ostream>
// #include <cstddef>

// using std::size_t;

// const static double BASE_WEIGHTS = 0.2;
// const static double DAMPENING = 0.5;

// class NeuralNetwork {
//     public:
//         NeuralNetwork(unsigned int);
//         NeuralNetwork(unsigned int, Activators::FunctionType ftype);
//         void Initialise();
//         void Step();
//         void FeedData();
//         void PrintInnerNodes(std::ostream& os);
//         void PrintOutputNodes(std::ostream& os, int index);
//         void AddInput(std::vector<double>* input_nodes);
//         void AddOutput(std::vector<double>* output_nodes);
        
//     private:

//         void StepInputNodes();
//         void StepInnerNodes();
//         void StepOutputNodes();
//         void Dampen();

//         bool initialised_;
//         size_t size_;
//         std::vector<double>* inner_nodes_;
//         SparseMatrix<double>* inner_weights_;
//         std::vector<double>* working_buffer_;
//         std::vector<double>* inner_biases_;

//         size_t n_inputs_;
//         std::vector<std::vector<double>*> input_nodes_;
//         std::vector<SparseMatrix<double>*> input_weights_;

//         size_t n_outputs_;
//         std::vector<std::vector<double>*> output_nodes_;
//         std::vector<SparseMatrix<double>*> output_weights_;

//         double (*activator_)(double);
//         double (*activator_derivative_)(double);
// };

// #endif // NEURALNETOWRK_H_