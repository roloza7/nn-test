#include "sparsematrix.h"
#include "neuralnetwork.h"
#include <iostream>
#include <ctime>
#include <random>

int main( void ) {

    std::random_device _rd{};
    std::mt19937 _gen{_rd()};
    std::uniform_real_distribution<double> _dist;

    std::uniform_real_distribution<double>::param_type param(0, 0.5);
    _dist.param(param);

    NeuralNetwork nn(10);
    nn.PrintInnerNodes(std::cout);
    std::vector<double>* inputs = new std::vector<double>(10, 0.2);
    std::vector<double>* inputs2 = new std::vector<double>(5, 0.5);
    std::vector<double>* output = new std::vector<double>(20, 0);
    nn.AddInput(inputs);
    nn.AddInput(inputs2);
    nn.AddOutput(output);
    for (unsigned i = 0; i < 6; i++) {
        for (double& d : *inputs)
            d = _dist(_gen);
        for (double& d : *inputs2)
            d = _dist(_gen);
        nn.Step();
        nn.PrintOutputNodes(std::cout, 0);
    }

}