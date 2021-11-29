#include "sparsematrix.h"
#include "neuralnetwork.h"
#include "trainerwrapper_impl.h"
#include <iostream>
#include <ctime>
#include <random>
#include <math.h>

void printVector(std::ostream& os, const std::vector<double>& v) {
    os << "[ ";
    for (const double& d : v)
        os << d << " ";
    os << "]" << std::endl;
}

int main( void ) {

    TrainerWrapper_impl tw(50, 15);
    tw.PrintInnerNodes();
    double** inputs = tw.AddInputArray(5); // 10 x 5
    double** outputs = tw.AddOutputArray(3); // 10 x 5
    for (size_t i = 0; i < 50; ++i) {
        for (size_t j = 0; j < 5; ++j)
            inputs[i][j] = j;
    }
    tw.NextGeneration(0);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 50; ++j) {
            tw.Step();
        }
        double min_dist = 100;
        double min_id = 0;
        for (int j = 0; j < 50; ++j) {
            double dist = std::pow(outputs[j][0] - 2, 2) + std::pow(outputs[j][1] - 2, 2) + std::pow(outputs[j][2], 2);
            if (dist < min_dist) {
                min_dist = dist;
                min_id = j;
            }
        }
        tw.NextGeneration(min_id);
        // std::cout << "[ ";
        // for (int j = 0; j < 5; j++)
        //     std::cout << outputs[(int)min_id][j] << " ";
        // std::cout << "]" << std::endl;;
        std::cout << min_dist << std::endl;
    }
    
}