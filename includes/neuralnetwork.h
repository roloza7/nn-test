#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include "activators.h"
#include "sparsematrix.h"
#include "inputbatch.h"
#include "outputbatch.h"

#include <ostream>
#include <cstddef>

using std::size_t;


class NeuralNetwork {
    public:
        NeuralNetwork(unsigned int);
        NeuralNetwork(unsigned int, Activators::FunctionType ftype);
        void SetConstants(double base_weights, double dampening, double plasticity,
                        double pruning, double synaptogenesis, const std::vector<double>& capacity);
        void Initialise();
        void Step();
        void FeedData();
        void PrintInnerNodes(std::ostream& os);
        void PrintOutputNodes(std::ostream& os, int index);
        void AddInput(std::vector<double>* input_nodes);
        void AddOutput(std::vector<double>* output_nodes);

        void StepInnerWeights();
        void CalculateSynapsesDiff();
        void GenerateSpontaneousSynapses();
        void CalculatePruning();
        
        InputBatch& GetInputNodes();
        OutputBatch& GetOutputNodes();

        SparseMatrix<double>& GetWeights();

    private:

        void Propagate();
        void Dampen();
        double InterpolateSynapse(double x, const double& p);

        void CalculateWeightChange();

        double BASE_WEIGHTS;
        double BASE_BIASES;
        double DAMPENING;
        double PLASTICITY;
        double PRUNING;
        double SYNAPTOGENESIS;
        std::vector<double> CAPACITY;

        bool initialised_;
        size_t size_;
        std::vector<double>* inner_nodes_;
        SparseMatrix<double>* inner_weights_;
        std::vector<double>* working_buffer_;
        std::vector<double>* inner_biases_;

        InputBatch input_nodes_;
        OutputBatch output_nodes_;

        double (*activator_)(double);
        double (*activator_derivative_)(double);

        std::mt19937 rng_;
        std::uniform_real_distribution<> urd_;
        std::uniform_int_distribution<> uid_;
};

#endif // NEURALNETOWRK_H_