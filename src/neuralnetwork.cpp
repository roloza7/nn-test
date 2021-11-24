#include "neuralnetwork.h"
#include <algorithm>
#include <cassert>
#include <cmath>
using std::vector;
using std::log;
using std::exp;
using std::max;
using std::min;

#pragma region Configs

NeuralNetwork::NeuralNetwork(unsigned size) : NeuralNetwork(size, Activators::FunctionType::BinaryStep) {};

NeuralNetwork::NeuralNetwork(unsigned size, Activators::FunctionType ftype) : initialised_(false), size_(size), input_nodes_(size), output_nodes_(size) {
    inner_nodes_ = new vector<double>(size_);
    working_buffer_ = new vector<double>(size_);
    inner_weights_ = new SparseMatrix<double>(size_, size_);
    inner_biases_ = new vector<double>(size_);

    std::pair<double (*)(double), double (*)(double)> activator_f_df = Activators::ActivatorMap.find(ftype)->second;
    activator_ = activator_f_df.first;
    activator_derivative_ = activator_f_df.second;

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

void NeuralNetwork::SetConstants(double base_weights, double dampening, double plasticity,
                        double pruning, double synaptogenesis, const std::vector<double>& capacity) {

    if (capacity.size() != size_)
        throw std::runtime_error("Capacity is not the same size as the network structure");

    BASE_WEIGHTS = base_weights;
    DAMPENING = dampening;
    PLASTICITY = plasticity;
    PRUNING = pruning;
    SYNAPTOGENESIS = synaptogenesis;
    CAPACITY = capacity;
    
    Initialise();

    initialised_ = true;
}

#pragma endregion Configs

#pragma region NodePropagation

void NeuralNetwork::Step() {
    assert(initialised_);
    // Reset working buffer
    std::fill(working_buffer_->begin(), working_buffer_->end(), 0);
    
    // Dampen the previous (temporal) outputs.
    Dampen();

    // Propagate inputs to inner nodes
    input_nodes_.Propagate(*working_buffer_);

    // Propagate inner nodes onto themselves
    Propagate();
    
    // Propagate inner nodes to output nodes
    output_nodes_.Propagate(*working_buffer_);

    /** 
     * At this point input_nodes_ stores the n_i iteration, and working_buffer_ stores n_(i+1)
     *  We now calculate weight changes based on neuron activation
     *  As a rule: neurons that fire together, stay together
     *  1. If synapse A -> B has nonzero weight, and B has not fired, then A -> B is weakened.
     *  2. Otherwise, A -> B strengthens.
     *  3. If A has enough capacity, it will try to randomly connect to other nodes.
     *  4. The system biases against smaller weights, with stronger connections fading more slowly.
     *  5. If a change to A -> B would put it under a specific threshold of absolute value, it is removed.
     * 
     *  These changes are governed by several constants:
     *  PLASTICITY. How much a connection can change in a given step.
     *  Plasticity also affects synapse creation and deletion.
     *  PRUNING. How much the system biases towards removing connections.
     *  Pruning affects synapse deletion.
     *  SYNAPTOGENESIS. How much a neuron want to spontenously form new connections.
     *  Synaptogenesis affects spontaneous synapse creation.
     *  CAPACITY. How many connections a given neuron can support.
     *  Capacity affects synapse creation. Does not affect weights. Negatively affects pruning if node is over encumbered.
     *  
     **/
}


/**
 *  Steps inner nodes.
 *  Switches between the node storage and a working buffer.
 * */
void NeuralNetwork::Propagate() {
    LinAlg::Multiply(*inner_nodes_, *inner_weights_, *working_buffer_);

    for (unsigned i = 0; i < size_; i++) {
        (*working_buffer_)[i] += (*inner_biases_)[i];
        (*working_buffer_)[i] = activator_((*working_buffer_)[i]);
    }
}

void NeuralNetwork::Dampen() {
    for (double& v : *inner_nodes_)
        v *= DAMPENING;
}

#pragma endregion NodePropagation

#pragma region Debug

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

InputBatch& NeuralNetwork::GetInputNodes() {
    return input_nodes_;
}
OutputBatch& NeuralNetwork::GetOutputNodes() {
    return output_nodes_;
}

const SparseMatrix<double>& NeuralNetwork::GetWeights() {
    return *inner_weights_;
}

#pragma endregion Debug

#pragma region Synapses

void NeuralNetwork::StepInnerWeights() {
    CalculateSynapsesDiff();
}

static const double c = log(3.85911496);
static const double denom = exp(-4*c) - 2*exp(-c) + 1;
static const double num = -2 * exp(-c);

double NeuralNetwork::InterpolateSynapse(double x, const double& p) {
    // interpolation constant
    bool sign = std::signbit(x); // true if negative

    // negative plasticity interpolates towards zero, otherwise one
    x = x + p * (sign ? -1 : 1);

    // clamp x to [-1, 1]
    x = max(min(x, 1.0), -1.0);

    // return will be in the interpolated range
    return ((exp(-c * (x - 1) * (x - 1)) + exp(-c * (-x - 1) * (-x - 1)) + num)/(denom)) * sign;
}

void NeuralNetwork::CalculateSynapsesDiff() {
    for (size_t y = 0; y < size_; ++y) {

        // If node didn't activate (i.e.: pass on its signal), don't calculate synapse differential
        if ((*inner_nodes_)[y] == 0)
            continue;

        // Iterate and change weights
        std::map<size_t, double>::iterator row = (*inner_weights_)[y].begin();

        for (; row != (*inner_weights_)[y].end(); ++row) {
            if ((*working_buffer_)[row->first] == 1)
                row->second = InterpolateSynapse(row->second, PLASTICITY);
            else
                row->second = InterpolateSynapse(row->second, -PLASTICITY);
        }

    }
}

void NeuralNetwork::GenerateSpontaneousSynapses() {

}

#pragma endregion Synapses
