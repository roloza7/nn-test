#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include "activators.h"
#include "sparsematrix.h"
#include "inputbatch.h"
#include "outputbatch.h"

#include <ostream>
#include <cstddef>

using std::size_t;

struct NetworkStructure {
        NetworkStructure() = default;
        NetworkStructure(size_t network_size);
        ~NetworkStructure();
        void Clear(size_t n_inputs, size_t n_outputs);
        size_t network_size_;
        vector<vector<double>*> inputs_capacity_;
        vector<SparseMatrix<double>*> inputs_weight_;
        vector<vector<double>*> outputs_capacity_;
        vector<SparseMatrix<double>*> outputs_weight_;
        vector<double> inner_capacity_;
        vector<double> inner_biases_;
        SparseMatrix<double> inner_weights_;
        double plasticity_;
        double dampening_;
        double pruning_;
        double synaptogenesis_;
        size_t n_inputs_;
        size_t n_outputs_;
};

typedef struct NetworkStructure NetworkStructure;
class NeuralNetwork {
    public:

        NeuralNetwork(unsigned int);
        NeuralNetwork(unsigned int, Activators::FunctionType ftype);
        ~NeuralNetwork();
        void SetConstants(double base_weights, double dampening, double plasticity,
                        double pruning, double synaptogenesis, const std::vector<double>& capacity);
        void SetConstants(double base_weights, double dampening, double plasticity,
                        double pruning, double synaptogenesis);
        void Initialise();
        void Step();
        void FeedData();
        void PrintInnerNodes(std::ostream& os);
        void PrintInnerWeights(std::ostream& os);
        void PrintInputNodes(std::ostream& os);
        void PrintInputWeights(std::ostream& os);
        void PrintInputCapacities(std::ostream& os);
        void PrintOutputNodes(std::ostream& os);
        void AddInput(std::vector<double>* input_nodes, std::vector<double>* capacity);
        void AddOutput(std::vector<double>* output_nodes, std::vector<double>* capacity);

        void StepWeights();
        
        InputBatch& GetInputNodes();
        OutputBatch& GetOutputNodes();

        SparseMatrix<double>& GetWeights();
        vector<double>& GetBiases();
        vector<double>& GetCapacities();

        void GetStructuralData(NetworkStructure* ns);
        void SetStructuralData(NetworkStructure* ns);

        double GetDampening();
        double Plasticity();
        double Pruning();
        double Synaptogenesis();

    private:

        void Propagate();
        void Dampen();
        

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

};



#endif // NEURALNETOWRK_H_