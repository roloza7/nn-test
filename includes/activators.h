#ifndef ACTIVATORS_H_
#define ACTIVATORS_H_

#include <map>

struct Activators {
    enum FunctionType : char { Logistic, HyperbolicTangent };
    struct Functions {
        static double Logistic(double x);
        static double HyperbolicTangent(double x);
    };
    struct Derivatives {
        static double Logistic(double x);
        static double HyperbolicTangent(double x);
    };
    const static std::map<FunctionType, std::pair<double (*)(double), double (*)(double)>> ActivatorMap;
};


#endif // ACTIVATORS_H_