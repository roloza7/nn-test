#pragma once

#include <vector>
#include "sparsematrix.h"

template class SparseMatrix<double>;

struct NetworkStructure;

using std::vector;

namespace Utils {

    void CalculateSynapsesDiff(const vector<double>& prev, SparseMatrix<double>& weights, const vector<double>& next, double PLASTICITY);

    void GenerateSpontaneousSynapses(SparseMatrix<double>& weights, vector<double>& capacity, double SYNAPTOGENESIS);

    void CalculatePruning(SparseMatrix<double>& weights, vector<double>& capacity, double PRUNING);    

    void Recombine(vector<NetworkStructure*>& structures, int best, int second_best, NetworkStructure* out);

    void CrossoverArray(vector<double>* arr1, vector<double>* arr2, int k, vector<double>* out);

    void CrossoverMatrix(SparseMatrix<double>* sm1, SparseMatrix<double>* sm2, int k, SparseMatrix<double>* out);

    double InterpolateSynapse(double x, const double& p);
}