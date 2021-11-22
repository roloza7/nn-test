#include "sparsematrix.h"
#include <iostream>
#include <ctime>
#include <random>

int main( void ) {

    SparseMatrix<double> r(4, 4);
    std::vector<double> l(4);
    
    for (unsigned i = 0; i < l.size(); ++i) {
        l[i] = i;
        for (unsigned j = 0; j < 4; ++j)
            r[i][j] = i;
    }
    
    std::cout << "[ ";
    for (const double& d : l)
        std::cout << d << " ";
    std::cout << "]" << std::endl;

    std::vector<double> o = LinAlg::Multiply(l, r);
    std::cout << "[ ";
    for (const double& d : o)
        std::cout << d << " ";
    std::cout << "]" << std::endl;


}