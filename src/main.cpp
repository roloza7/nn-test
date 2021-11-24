#include "sparsematrix.h"
#include "neuralnetwork.h"
#include <iostream>
#include <ctime>
#include <random>

void printVector(std::ostream& os, const std::vector<double>& v) {
    os << "[ ";
    for (const double& d : v)
        os << d << " ";
    os << "]" << std::endl;
}

int main( void ) {

    NeuralNetwork nn(4);
    std::vector<double>* input = new vector<double>({1, 0});
    nn.AddInput(input);
    InputBatch& inputBatch = nn.GetInputNodes();
    SparseMatrix<double>& inputWeights = inputBatch.GetWeight(0);
    inputWeights[0][0] = 1.0;
    inputWeights[1][2] = 1.0;
    std::cout << "Inputs:\n";
    printVector(std::cout, *input);
    std::cout << "Input Weights:\n";
    std::cout << inputWeights;
    std::cout << "Inner Nodes:\n";
    nn.PrintInnerNodes(std::cout);
    std::cout << "Inner Weights:\n";
    const SparseMatrix<double>& innerWeights = nn.GetWeights();
    std::cout << innerWeights;


}