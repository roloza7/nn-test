#ifndef ACTIVATORS_H_
#define ACTIVATORS_H_

#include <map>

struct Activators {
    enum FunctionType : char { Logistic, HyperbolicTangent, BinaryStep };
    struct Functions {
        static double Logistic(double x);
        static double HyperbolicTangent(double x);
        static double BinaryStep(double x); 
    };
    struct Derivatives {
        static double Logistic(double x);
        static double HyperbolicTangent(double x);
        static double BinaryStep(double x);
    };
    const static std::map<FunctionType, std::pair<double (*)(double), double (*)(double)>> ActivatorMap;
};


#endif // ACTIVATORS_H_