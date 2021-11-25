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
    nn.SetConstants(0, 1, 0.2, 0.006, 1, std::vector<double>(4, 3));
    std::vector<double>* input = new vector<double>({1, 0});
    nn.AddInput(input);
    InputBatch& inputBatch = nn.GetInputNodes();
    SparseMatrix<double>& inputWeights = inputBatch.GetWeight(0);
    SparseMatrix<double>& innerWeights = nn.GetWeights();
    innerWeights[0][1] = 2.0;
    innerWeights[0][3] = 0.2;
    inputWeights[0][0] = 1.0;
    inputWeights[1][3] = -1.0;
    std::cout << "Inputs:\n";
    printVector(std::cout, *input);
    std::cout << "Input Weights:\n";
    std::cout << inputWeights;
    std::cout << "Inner Nodes:\n";
    nn.PrintInnerNodes(std::cout);
    std::cout << "Inner Weights:\n";
    std::cout << innerWeights;


    for (int i = 0; i < 100; ++i) {
        if (i == 5) {
            std::cout << "========= INPUTS CHANGED =========\n";
            (*input)[1] = 1;
        }
        std::cout << "========= STEP =========\n";
        nn.Step();
        std::cout << "Inputs:\n";
        printVector(std::cout, *input);
        std::cout << "Input Weights:\n";
        std::cout << inputWeights;
        std::cout << "Inner Nodes:\n";
        nn.PrintInnerNodes(std::cout);
        std::cout << "Inner Weights:\n";
        std::cout << innerWeights;
    }
    
}