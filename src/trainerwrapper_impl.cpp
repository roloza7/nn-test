#include "trainerwrapper_impl.h"
#include "neuralnetwork.h"
#include "inputbatch.h"
#include "outputbatch.h"
#include "utils.h"
#include <random>
#include <math.h>
#include <iostream>
#include <sstream>
//#include "debugcallback.hpp"


#ifndef TrainerWrapper
#define TrainerWrapper TrainerWrapper_impl
#endif

using std::vector;

TrainerWrapper::TrainerWrapper(int n_subjects, int network_size) {
    network_size_ = network_size;
    n_subjects_ = n_subjects;
    n_inputs_ = 0;
    n_outputs_ = 0;
  
    recombination_buffer_ = new NetworkStructure(network_size_);
    networks_starting_info_ = new vector<NetworkStructure*>();
    for (size_t i = 0; i < n_subjects_; ++i) {
        networks_starting_info_->push_back(new NetworkStructure(network_size_));
    }
    InitializeRandomChildren();


}

TrainerWrapper::~TrainerWrapper() {
    for (size_t i = 0; i < n_subjects_; ++i) {
        delete networks_[i];
        delete (*networks_starting_info_)[i];
    }

    for (auto it = inputs_.begin(); it != inputs_.end(); ++it) {
        for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
            delete* it2;
        }
    }

    for (auto it = outputs_.begin(); it != outputs_.end(); ++it) {
        for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
            delete* it2;
        }
    }
}

double** TrainerWrapper::AddInputArray(int input_size) {
    double** inputs = new double* [n_subjects_];
    vector<vector<double>*> input_layer(n_subjects_, nullptr);
    for (size_t i = 0; i < n_subjects_; ++i) {
        input_layer[i] = new vector<double>(input_size, 0);
        inputs[i] = input_layer[i]->data();
    }

    for (size_t i = 0; i < n_subjects_; ++i) {
        networks_[i]->AddInput(input_layer[i], new vector<double>(input_size, (int)std::sqrt(network_size_)));
    }


    inputs_.push_back(input_layer);

    return inputs;

}

double** TrainerWrapper::AddOutputArray(int output_size) {
    double** outputs = new double* [n_subjects_];
    vector<vector<double>*> output_layer(n_subjects_, nullptr);
    for (size_t i = 0; i < n_subjects_; ++i) {
        output_layer[i] = new vector<double>(output_size, 0);
        outputs[i] = output_layer[i]->data();
    }

    for (size_t i = 0; i < n_subjects_; ++i) {
        networks_[i]->AddOutput(output_layer[i], new vector<double>(network_size_, std::sqrt(output_size)));
    }


    outputs_.push_back(output_layer);

    return outputs;
}

void TrainerWrapper::Step() {
    for (vector<NeuralNetwork*>::iterator it = networks_.begin(); it != networks_.end(); ++it) {
        (*it)->Step();
    }
}


void TrainerWrapper::NextGeneration(int winner_id, int runner_up_id) {

    for (size_t i = 0; i < n_subjects_; ++i) {
        if (i == size_t(winner_id) || i == size_t(runner_up_id))
            continue;
        Utils::Recombine(*networks_starting_info_, winner_id, runner_up_id, recombination_buffer_);
        networks_[i]->SetStructuralData(recombination_buffer_);
    }

    CreateRandomChildren();

}

static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_real_distribution<double> urd(-0.02, 0.02);
static std::uniform_int_distribution<int> uid(-2, 2);
static std::uniform_real_distribution<double> urd2(-0.05, 0.05);

void TrainerWrapper::CreateRandomChildren() {

    //DebugInUnity(ss.str());

    for (size_t i = 0; i < n_subjects_; ++i) {

        
        if (i == 0) {
            // networks_[i]->PrintInnerWeights(std::cout);
            // networks_[i]->PrintInputCapacities(std::cout);
            // networks_[i]->PrintInputWeights(std::cout);
        }

        // setup randommized steps
        double new_plasticity = networks_[i]->Plasticity() + urd(mt);
        double new_pruning = networks_[i]->Pruning() + urd(mt);
        double new_dampening = networks_[i]->GetDampening() + urd(mt);
        double new_synaptogenesis = networks_[i]->Synaptogenesis() + urd(mt);

        // check random capacities
        vector<double>& base_capacities = networks_[i]->GetCapacities();

        for (size_t j = 0; j < base_capacities.size(); ++j) {
            base_capacities[j] = std::min(std::max(base_capacities[j] + uid(mt), 1.0), 10.0);
        }

        // create constants
        networks_[i]->SetConstants(0, 1, new_plasticity, new_pruning, new_synaptogenesis);


        // setup stepped inner weights
        SparseMatrix<double>& inner_weights = networks_[i]->GetWeights();
        for (size_t y = 0; y < inner_weights.Height(); ++y) {
            SparseMatrix<double>::RowProxy row = inner_weights.operator[](y);
            for (size_t k = 0; k < std::ceil(double(inner_weights.Width()) / 20.0); ++k) {
                int idx = mt() % inner_weights.Width();
                row[idx] = urd2(mt);
            }
        }

        // setup stepped inner biases
        vector<double>& base_biases = networks_[i]->GetBiases();
        for (size_t j = 0; j < base_biases.size(); ++j) {
            base_biases[j] = std::max(-2.0, std::min(base_biases[j] + urd2(mt), 2.0));
        }

        InputBatch& curr_inputs = networks_[i]->GetInputNodes();
        OutputBatch& curr_outputs = networks_[i]->GetOutputNodes();

        // setup stepped input capacities
        vector<vector<double>*>& inputs_cap = curr_inputs.GetCapacities();


        for (size_t k = 0; k < inputs_cap.size(); ++k) {
            for (size_t j = 0; j < inputs_cap[k]->size(); ++j) {
                inputs_cap[k]->operator[](j) = std::min(std::max(1.0, inputs_cap[k]->operator[](j) + uid(mt)), 10.0);
            }
        }
        // setup stepped input weights
        vector<SparseMatrix<double>*>& inputs_weights = curr_inputs.GetWeights();
        for (size_t k = 0; k < inputs_weights.size(); ++k) {
            for (size_t y = 0; y < inputs_weights[k]->Height(); ++y) {
                SparseMatrix<double>::RowProxy row = inputs_weights[k]->operator[](y);
                for (size_t j = 0; j < std::ceil(double(inputs_weights[k]->Width()) / 20.0); ++j) {
                    int idx = mt() % inputs_weights[k]->Width();
                    row[idx] = urd2(mt);
                }
            }
        }
        // setup stepepd output caps
        vector<vector<double>*>& outputs_cap = curr_outputs.GetCapacities();
        for (size_t k = 0; k < outputs_cap.size(); ++k) {
            for (size_t j = 0; j < outputs_cap[k]->size(); ++j) {
                outputs_cap[k]->operator[](j) = std::min(std::max(1.0, outputs_cap[k]->operator[](j) + uid(mt)), 10.0);
            }
        }
        // setup stepped output weights
        vector<SparseMatrix<double>*>& outputs_weights = curr_outputs.GetWeights();
        for (size_t k = 0; k < outputs_weights.size(); ++k) {
            for (size_t y = 0; y < outputs_weights[k]->Height(); ++y) {
                SparseMatrix<double>::RowProxy row = outputs_weights[k]->operator[](y);
                for (size_t j = 0; j < std::ceil(double(outputs_weights[k]->Width()) / 20.0); ++j) {
                    int idx = mt() % outputs_weights[k]->Width();
                    row[idx] = urd2(mt);
                }
            }
        }
        networks_[i]->Initialise();
        networks_[i]->GetStructuralData((*networks_starting_info_)[i]);

    }




}

void TrainerWrapper::InitializeRandomChildren() {
    
    for (size_t i = 0; i < n_subjects_; ++i) {
        networks_.push_back(new NeuralNetwork(network_size_));
        networks_[i]->SetConstants(0, 1, .2, 0.006, 1, std::vector<double>(network_size_, std::sqrt(network_size_)));
        networks_[i]->GetStructuralData((*networks_starting_info_)[i]);
    }
    networks_[0]->GetStructuralData(recombination_buffer_);
}

void TrainerWrapper::PrintInnerNodes() {
    for (size_t i = 0; i < n_subjects_; ++i)
        networks_[i]->PrintInnerNodes(std::cout);
}

void TrainerWrapper::PrintInputNodes() {
    for (size_t i = 0; i < n_subjects_; ++i)
        networks_[i]->PrintInputNodes(std::cout);
}

void TrainerWrapper::PrintInputWeights() {
    for (size_t i = 0; i < n_subjects_; ++i)
        networks_[i]->PrintInputWeights(std::cout);
}

void TrainerWrapper::PrintInputCapacities() {   
    for (size_t i = 0; i < n_subjects_; ++i) {
        networks_[i]->PrintInputCapacities(std::cout);
    }
}

void TrainerWrapper::PrintOutputNodes() {
    for (size_t i = 0; i < n_subjects_; ++i) {
        networks_[i]->PrintOutputNodes(std::cout);
    }
}
