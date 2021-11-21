#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include "activators.h"
#include "sparsematrix.h"

#include <ostream>
#include <cstddef>

using std::size_t;

const static double BASE_WEIGHTS = 0.5;

class NeuralNetwork {
    public:
        NeuralNetwork(unsigned int);
        NeuralNetwork(unsigned int, Activators::FunctionType ftype);
        void Initialise();
        void Step();
        void FeedData();
        void PrintInnerNodes(std::ostream& os);
        
    private:

        void StepInputNodes();
        void StepInnerNodes();
        void StepOutputNodes();

        bool initialised_;
        size_t size_;
        std::vector<double>* inner_nodes_;
        SparseMatrix<double>* inner_weights_;
        std::vector<double>* working_buffer_;
        
        std::vector<double>* inner_biases_;
        double (*activator_)(double);
        double (*activator_derivative_)(double);
};

#endif // NEURALNETOWRK_H_