#include "neuralnetwork.h"
#include "sparsematrix.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
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
    std::fill(inner_nodes_->begin(), inner_nodes_->end(), 0);

    initialised_ = true;
}

void NeuralNetwork::AddInput(vector<double>* input_nodes, vector<double>* capacity) {
    input_nodes_.AddSeries(input_nodes, capacity);
}

void NeuralNetwork::AddOutput(vector<double>* output_nodes, vector<double>* capacity) {
    output_nodes_.AddSeries(output_nodes, capacity);
}

void NeuralNetwork::SetConstants(double base_weights, double dampening, double plasticity,
    double pruning, double synaptogenesis, const std::vector<double>& capacity) {

    if (capacity.size() != size_)
        throw std::runtime_error("Capacity is not the same size as the network structure");

    for (const double& d : capacity)
        assert(d != 0);

    BASE_WEIGHTS = base_weights;
    BASE_BIASES = 0;
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
    StepWeights();




    vector<double>* t = inner_nodes_;
    inner_nodes_ = working_buffer_;
    working_buffer_ = t;
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

void NeuralNetwork::PrintOutputNodes(std::ostream& os) {
    const vector<vector<double>*>& series = output_nodes_.GetSeries();



    for (auto it = series.begin(); it != series.end(); ++it) {
        os << "[ ";
        for (size_t i = 0; i < (*it)->size(); ++i)
            os << (*it)->operator[](i) << " ";
        os << "]";
    }
    os << std::endl;

}

InputBatch& NeuralNetwork::GetInputNodes() {
    return input_nodes_;
}
OutputBatch& NeuralNetwork::GetOutputNodes() {
    return output_nodes_;
}

SparseMatrix<double>& NeuralNetwork::GetWeights() {
    return *inner_weights_;
}

#pragma endregion Debug

#pragma region Synapses

void NeuralNetwork::StepWeights() {
    // Step input weights
    vector<vector<double>*>& inputs = input_nodes_.GetSeries();
    vector<SparseMatrix<double>*>& inputs_weights = input_nodes_.GetWeights();
    vector<vector<double>*>& capacities = input_nodes_.GetCapacities();
    for (size_t s = 0; s < inputs.size(); ++s) {

        Utils::CalculateSynapsesDiff(*inputs[s], *inputs_weights[s], *working_buffer_, PLASTICITY);
        Utils::CalculatePruning(*inputs_weights[s], *capacities[s], PRUNING);
        Utils::GenerateSpontaneousSynapses(*inputs_weights[s], *capacities[s], SYNAPTOGENESIS);

    }

    // Step inner weights
    Utils::CalculateSynapsesDiff(*inner_nodes_, *inner_weights_, *working_buffer_, PLASTICITY);
    Utils::CalculatePruning(*inner_weights_, CAPACITY, PRUNING);
    Utils::GenerateSpontaneousSynapses(*inner_weights_, CAPACITY, SYNAPTOGENESIS);

    const vector<vector<double>*>& outputs = output_nodes_.GetSeries();
    vector<SparseMatrix<double>*>& outputs_weights = output_nodes_.GetWeights();
    vector<vector<double>*>& outputs_capacities = output_nodes_.GetCapacities();
    for (size_t s = 0; s < outputs.size(); ++s) {

        Utils::CalculateSynapsesDiff(*inner_nodes_, *outputs_weights[s], *outputs[s], PLASTICITY);
        Utils::CalculatePruning(*outputs_weights[s], *outputs_capacities[s], PRUNING);
        Utils::GenerateSpontaneousSynapses(*outputs_weights[s], *outputs_capacities[s], SYNAPTOGENESIS);

    }
}

#pragma endregion Synapses

NeuralNetwork::~NeuralNetwork() {
    delete inner_nodes_;
    delete inner_weights_;
    delete inner_biases_;
    delete working_buffer_;
}

vector<double>& NeuralNetwork::GetBiases() {
    return *inner_biases_;
}

double NeuralNetwork::GetDampening() {
    return DAMPENING;
}
double NeuralNetwork::Plasticity() {
    return PLASTICITY;
}
double NeuralNetwork::Pruning() {
    return PRUNING;
}
double NeuralNetwork::Synaptogenesis() {
    return SYNAPTOGENESIS;
}

vector<double>& NeuralNetwork::GetCapacities() {
    return CAPACITY;
}

void NeuralNetwork::PrintInnerWeights(std::ostream& os) {
    os << "=== INNER WEIGHTS ===\n";
    os << *inner_weights_;
}

void NeuralNetwork::PrintInputNodes(std::ostream& os) {
    os << "=== INPUTS ===\n";
    InputBatch& ib = GetInputNodes();
    vector<vector<double>*>& inputs = ib.GetSeries();
    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        os << "[ ";
        for (auto it2 = (*it)->begin(); it2 != (*it)->end(); ++it2)
            os << *it2 << " ";
        os << "]\n";
    }
    os << std::endl;
}
void NeuralNetwork::PrintInputWeights(std::ostream& os) {
    os << "=== INPUT WEIGHTS ===\n";
    InputBatch& ib = GetInputNodes();
    vector<SparseMatrix<double>*>& weights = ib.GetWeights();
    for (auto it = weights.begin(); it != weights.end(); ++it) {
        os << **it;
    }
    os << std::endl;
}

void NeuralNetwork::PrintInputCapacities(std::ostream& os) {
    os << "=== INPUT CAPACITIES ===\n";
    InputBatch& ib = GetInputNodes();
    vector<vector<double>*>& capacities = ib.GetCapacities();
    for (auto it = capacities.begin(); it != capacities.end(); ++it) {
        os << "[ ";
        for (size_t i = 0; i < (*it)->size(); ++i)
            os << (**it)[i] << " ";
        os << "]\n";
    }
}

void NeuralNetwork::SetConstants(double base_weights, double dampening, double plasticity,
    double pruning, double synaptogenesis) {
    BASE_WEIGHTS = base_weights;
    DAMPENING = dampening;
    PLASTICITY = plasticity;
    PRUNING = pruning;
    SYNAPTOGENESIS = synaptogenesis;
}

void NeuralNetwork::GetStructuralData(NetworkStructure* ns) {

    // Set constants
    ns->dampening_ = DAMPENING;
    ns->plasticity_ = PLASTICITY;
    ns->pruning_ = PRUNING;
    ns->synaptogenesis_ = SYNAPTOGENESIS;
    ns->network_size_ = size_;

    ns->inner_capacity_ = CAPACITY;
    ns->inner_biases_ = *inner_biases_;
    ns->inner_weights_.Set(*inner_weights_);

    const vector<vector<double>*>& current_input_cap = input_nodes_.GetCapacities();
    const vector<vector<double>*>& current_output_cap = output_nodes_.GetCapacities();
    const vector<SparseMatrix<double>*>& current_input_weights = input_nodes_.GetWeights();
    const vector<SparseMatrix<double>*>& current_output_weights = output_nodes_.GetWeights();

    if (ns->inputs_capacity_.size() != current_input_cap.size() ||
        ns->outputs_capacity_.size() != current_output_cap.size()) {

        ns->Clear(current_input_cap.size(), current_output_cap.size());
    }

    for (size_t i = 0; i < ns->n_inputs_; ++i) {
        *ns->inputs_capacity_[i] = *current_input_cap[i];
        (ns->inputs_weight_[i])->Set(*current_input_weights[i]);
    }

    for (size_t i = 0; i < ns->n_outputs_; ++i) {
        *ns->outputs_capacity_[i] = *current_output_cap[i];
        (ns->outputs_weight_[i])->Set(*current_output_weights[i]);
    }

}

void NeuralNetwork::SetStructuralData(NetworkStructure* ns) {

    if (size_ != ns->network_size_)
        throw std::runtime_error("Invlaid network size");

    DAMPENING = ns->dampening_;
    PLASTICITY = ns->plasticity_;
    PRUNING = ns->pruning_;
    SYNAPTOGENESIS = ns->synaptogenesis_;

    CAPACITY = ns->inner_capacity_;
    *inner_biases_ = ns->inner_biases_;
    inner_weights_->Set(ns->inner_weights_);

    vector<vector<double>*>& current_input_cap = input_nodes_.GetCapacities();
    vector<vector<double>*>& current_output_cap = output_nodes_.GetCapacities();
    vector<SparseMatrix<double>*>& current_input_weights = input_nodes_.GetWeights();
    vector<SparseMatrix<double>*>& current_output_weights = output_nodes_.GetWeights();

    for (size_t i = 0; i < ns->n_inputs_; ++i) {
        *current_input_cap[i] = *ns->inputs_capacity_[i];
        current_input_weights[i]->Set(*ns->inputs_weight_[i]);
    }

    for (size_t i = 0; i < ns->n_outputs_; ++i) {
        *current_output_cap[i] = *ns->outputs_capacity_[i];
        current_output_weights[i]->Set(*ns->outputs_weight_[i]);
    }

}

void NetworkStructure::Clear(size_t n_inputs, size_t n_outputs) {

    for (size_t i = n_inputs; i < n_inputs_; ++i) {
        delete inputs_capacity_[i];
        delete inputs_weight_[i];
    }

    inputs_capacity_.reserve(n_inputs);
    inputs_weight_.reserve(n_inputs);

    for (size_t i = n_inputs_; i < n_inputs; ++i) {
        inputs_capacity_[i] = new vector<double>();
        inputs_weight_[i] = new SparseMatrix<double>();
    }

    for (size_t i = n_outputs; i < n_outputs_; ++i) {
        delete outputs_capacity_[i];
        delete outputs_weight_[i];
    }

    outputs_capacity_.reserve(n_outputs);
    outputs_weight_.reserve(n_outputs);

    for (size_t i = n_outputs_; i < n_outputs; ++i) {
        outputs_capacity_[i] = new vector<double>();
        outputs_weight_[i] = new SparseMatrix<double>();
    }

    n_inputs_ = n_inputs;
    n_outputs_ = n_outputs;

}

NetworkStructure::NetworkStructure(size_t network_size) {
    network_size_ = network_size;
    n_inputs_ = 0;
    n_outputs_ = 0;
}

NetworkStructure::~NetworkStructure() {
    Clear(0, 0);
}