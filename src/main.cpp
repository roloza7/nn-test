#include "sparsematrix.h"
#include "neuralnetwork.h"
#include <iostream>
#include <ctime>
#include <random>

int main( void ) {

    NeuralNetwork nn(10);
    nn.PrintInnerNodes(std::cout);
    std::vector<double>* input = new vector<double>({0, 0, 0, 0});
    nn.AddInput(input);
    std::vector<double>* output = new vector<double>(10);
    nn.AddOutput(output);
    nn.Step();
    nn.PrintInnerNodes(std::cout);
}