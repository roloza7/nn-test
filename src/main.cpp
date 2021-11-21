#include "sparsematrix.h"
#include "neuralnetwork.h"
#include <iostream>

int main( void ) {
    
    NeuralNetwork nn(10);
    nn.PrintInnerNodes(std::cout);
    for (unsigned i = 0; i < 6; i++) {
        nn.Step();
        nn.PrintInnerNodes(std::cout);
    }

}