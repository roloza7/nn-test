#include "utils.h"
#include "neuralnetwork.h"
#include <math.h>
#include <stdexcept>
#include <iostream>
#include <random>
#include <algorithm>

static const double c = 4.5;
static const double mult = 2.0 * std::sqrt(c / 3.14159265);

using std::min;
using std::max;

/**
 * Steps synapse towards/away from zero according to the gaussian integral 
 * 
 * @param x start point
 * @param p step (negative -> towards zero)
 * @return double 
 */
double Utils::InterpolateSynapse(double x, const double& p) {
    // interpolation constant
    bool sign = (std::signbit(x) == false ? 1 : -1);// tr ue if negative

    /**
     *  x > 0, p > 0 -> step forward
     *  x < 0, p > 0 -> step backward
     *  x > 0, p < 0 -> step backward
     *  x < 0, p > 0 -> step forward
     */

    // Calculate midpoint riemann sum
    double mid = x + (p/2.0) * sign;
    double dy = mult * (exp(-c * (mid - 1) * (mid - 1)) + exp(-c * (mid + 1) * (mid + 1))) * p * sign;

    // if it crosses y = x = 0, return 0. This is a guard against high plasticity relative to pruning.  
    if (std::signbit(x) != std::signbit(x + dy))
        return 0;
    // return will be in the interpolated range
    return min(max(x + dy, -2.0), 2.0);
}

void Utils::CalculateSynapsesDiff(const vector<double>& prev, SparseMatrix<double>& weights, const vector<double>& next, double PLASTICITY) {

    for (size_t y = 0; y < weights.Height(); ++y) {

        // If node didn't activate (i.e.: pass on its signal), don't calculate synapse differential
        if (prev[y] == 0) {
            continue;
        }

        // Iterate and change weights
        std::map<size_t, double>::iterator row = weights[y].begin();

        for (; row != weights[y].end(); ++row) {
            if (next[row->first] == 1)
                row->second = InterpolateSynapse(row->second, PLASTICITY);
            else
                row->second = InterpolateSynapse(row->second, -PLASTICITY);
        }

    }

}

static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_real_distribution urd(0.0, 1.0);

void Utils::GenerateSpontaneousSynapses(SparseMatrix<double>& weights, vector<double>& capacity, double SYNAPTOGENESIS) {

    for (size_t y = 0; y < weights.Height(); ++y) {

        SparseMatrix<double>::RowProxy row_proxy = weights[y];
        double capacity_ratio = ((double) row_proxy.size()) / capacity[y];
        
        /**
         * rng generates a random real number between 0 and 1
         * 
         */

        double random_num = urd(mt) * SYNAPTOGENESIS;
        if (random_num > capacity_ratio) {
            size_t idx = mt() % weights.Width();
            if (row_proxy[idx] == 0)
                row_proxy[idx] = urd(mt) - 0.5;
        }

    }

}

void Utils::CalculatePruning(SparseMatrix<double>& weights, vector<double>& capacity, double PRUNING) {

    for (size_t y = 0; y < weights.Height(); ++y) {
        SparseMatrix<double>::RowProxy row_proxy = weights[y];
        double capacity_ratio = ((double) row_proxy.size()) / capacity[y];
        capacity_ratio = std::min(capacity_ratio, 1.0); // clamp to 1 if under capacity
        std::map<size_t, double>::iterator row = row_proxy.begin();
        for (; row != row_proxy.end();) {
            auto next_row = row;
            next_row++;
            if (std::fabs(row->second) < PRUNING * capacity_ratio) // higher capacity ratios mean higher pruning chance
                weights[y].erase(row->first);
            row = next_row;
        }
    }

}

static int rec_factor = 3;

void Utils::Recombine(vector<NetworkStructure*>& structures, int best, int second_best, NetworkStructure* out) {

    for (size_t i = 0; i < out->inputs_capacity_.size(); ++i) {
        CrossoverArray(structures[best]->inputs_capacity_[i], structures[second_best]->inputs_capacity_[i], rec_factor, out->inputs_capacity_[i]);
        CrossoverMatrix(structures[best]->inputs_weight_[i], structures[second_best]->inputs_weight_[i], rec_factor, out->inputs_weight_[i]);
    }

    for (size_t i = 0; i < out->outputs_capacity_.size(); ++i) {
        CrossoverArray(structures[best]->outputs_capacity_[i], structures[second_best]->outputs_capacity_[i], rec_factor, out->outputs_capacity_[i]);
        CrossoverMatrix(structures[best]->outputs_weight_[i], structures[second_best]->outputs_weight_[i], rec_factor, out->outputs_weight_[i]);
    }

    CrossoverArray(&structures[best]->inner_capacity_, &structures[second_best]->inner_capacity_, rec_factor, &out->inner_capacity_);
    CrossoverArray(&structures[best]->inner_biases_, &structures[second_best]->inner_biases_, rec_factor, &out->inner_biases_);
    CrossoverMatrix(&structures[best]->inner_weights_, &structures[second_best]->inner_weights_, rec_factor, &out->inner_weights_);

    std::uniform_real_distribution uid{0.0, 1.0};

    double fac = uid(mt);
    out->plasticity_ = fac * structures[best]->plasticity_ + (1 - fac) * structures[second_best]->plasticity_;
    fac = uid(mt);
    out->dampening_ = fac * structures[best]->dampening_ + (1 - fac) * structures[second_best]->dampening_;
    fac = uid(mt);
    out->pruning_ = fac * structures[best]->pruning_ + (1 - fac) * structures[second_best]->pruning_;
    fac = uid(mt);
    out->synaptogenesis_ = fac * (structures[best]->synaptogenesis_ + (1 - fac) *  structures[second_best]->synaptogenesis_);

}



void Utils::CrossoverArray(vector<double>* arr1, vector<double>* arr2, int k, vector<double>* out) {

    if (arr1->size() != arr2->size() || arr1->size() != out->size())
        throw std::runtime_error("Crossover arrays must be the same size");

    std::uniform_int_distribution uid{(size_t) 0, (size_t)(arr1->size() - 1)};

    vector<size_t> crossover_points;
    for (int i = 0; i < k; ++i) {
        size_t idx = uid(mt);
        crossover_points.push_back(idx);
    }

    std::sort(crossover_points.begin(), crossover_points.end());
    vector<size_t>::iterator end = std::unique(crossover_points.begin(), crossover_points.end());

    bool picker = false;

    vector<size_t>::iterator it = crossover_points.begin();

    for (size_t i = 0; i < out->size(); ++i) {
        if (it != end && *it == i) {
            picker = !picker;
            ++it;
        }

        if (picker == false) {
            (*out)[i] = (*arr1)[i];
        } else {
            (*out)[i] = (*arr2)[i];
        }

    }

}

void Utils::CrossoverMatrix(SparseMatrix<double>* sm1, SparseMatrix<double>* sm2, int k, SparseMatrix<double>* out) {
    
    if (sm1->Height() != sm2->Height() || sm1->Width() != sm2->Width())
        throw std::runtime_error("Matrices are not the same size for crossover.");

    out->SetHeight(sm1->Height());
    out->SetWidth(sm1->Width());

    std::uniform_int_distribution uid{(size_t) 0 , sm1->Height() * sm1->Width() - 1};

    vector<size_t> crossover_points;
    for (int i = 0; i < k; ++i)
        crossover_points.push_back(uid(mt));

    std::sort(crossover_points.begin(), crossover_points.end());
    vector<size_t>::iterator end = std::unique(crossover_points.begin(), crossover_points.end());
    vector<size_t>::iterator it = crossover_points.begin();

    bool picker = false;

    std::map<size_t, double>::iterator row_writer = (*sm1)[0].begin();
    int last_row = sm1->Height() - 1;
    int row = 0;

    std::map<size_t, double>::iterator row1_it = (*sm1)[0].begin();
    std::map<size_t, double>::iterator row2_it = (*sm2)[0].begin();
    row_writer = row1_it;

    while (row_writer != (*sm1)[last_row].end() && row_writer != (*sm2)[last_row].end()) {
        
        if (row_writer == (*sm1)[row].end() || row_writer == (*sm2)[row].end()) {
            ++row;
            row1_it = (*sm1)[row].begin();
            row2_it = (*sm2)[row].begin();
            row_writer = (picker == false ? row1_it : row2_it);
            continue;
        }

        if (it != end && row * sm1->Width() + row_writer->first > *it) {
            if (picker == false) {// row1_it -> row2_it
                picker = true;
                row_writer = row2_it;
                while (row_writer != (*sm2)[row].end() && row * sm1->Width() + row_writer->first < *it)
                    ++row_writer;
                ++it;
            } else {
                picker = false;
                row_writer = row1_it;
                while (row_writer != (*sm1)[row].end() && row * sm1->Width() + row_writer->first < *it)
                    ++row_writer;
                ++it;
            }
        }

        if (row_writer != (*sm1)[row].end() && row_writer != (*sm2)[row].end()) {
            (*out)[row][row_writer->first] = row_writer->second;
            ++row_writer;
        }
    }

    
}