#include "sparsematrix.h"
#include "neuralnetwork.h"
#include "trainerwrapper_impl.h"
#include <iostream>
#include <ctime>
#include <random>
#include <math.h>
#include "utils.h"

void printVector(std::ostream& os, const std::vector<double>& v) {
    os << "[ ";
    for (const double& d : v)
        os << d << " ";
    os << "]" << std::endl;
}

int main( void ) {

    TrainerWrapper_impl tw(50, 25);
    tw.PrintInnerNodes();
    double** inputs = tw.AddInputArray(5); // 10 x 5
    double** outputs = tw.AddOutputArray(3); // 10 x 5
    for (size_t i = 0; i < 50; ++i) {
        for (size_t j = 0; j < 5; ++j)
            inputs[i][j] = j;
    }
    tw.NextGeneration(0, 0);
    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < 50; ++j) {
            tw.Step();
        }
        double min_dist = 10000000;
        double min_id = 0;
        double second_min_id = 0;
        for (int j = 0; j < 50; ++j) {
            double dist = std::pow(outputs[j][0] - 2, 2) + std::pow(outputs[j][1] - 2, 2) + std::pow(outputs[j][2], 2);
            if (dist < min_dist) {
                min_dist = dist;
                second_min_id = min_id;
                min_id = j;
            }
        }
        tw.NextGeneration(min_id, second_min_id);
        // std::cout << "[ ";
        // for (int j = 0; j < 5; j++)
        //     std::cout << outputs[(int)min_id][j] << " ";
        // std::cout << "]" << std::endl;;
        std::cout << min_dist << std::endl;
    }

    // std::vector<double> v1(20, 1);
    // std::vector<double> v2(20, 2);
    // std::vector<double> out(20, 0);

    // Utils::CrossoverArray(&v1, &v2, 4, &out);

    // std::cout << "[ ";
    // for (const double& d : out)
    //     std::cout << d << " ";
    // std::cout << "]" << std::endl;

    // SparseMatrix<double> sm1(10, 10);
    // SparseMatrix<double> sm2(10, 10);
    // SparseMatrix<double> smout(10, 10);

    // std::random_device rd;
    // std::mt19937 mt(rd());
    // std::uniform_real_distribution urd(-1.0, 1.0);

    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         sm1[i][j] = 0;
    //         sm2[i][j] = 1;
    //     }
    // }

    // Utils::CrossoverMatrix(&sm1, &sm2, 4, &smout);

    // std::cout << sm1 << std::endl;
    // std::cout << sm2 << std::endl;

    // std::cout << smout << std::endl;
    
}