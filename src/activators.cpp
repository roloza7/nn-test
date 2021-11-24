#include "activators.h"

#include <math.h>
#include <cmath>


const std::map<Activators::FunctionType, std::pair<double (*)(double), double (*)(double)>> Activators::ActivatorMap = {
        { Activators::FunctionType::Logistic, {&Activators::Functions::Logistic, &Activators::Derivatives::Logistic}},
        { Activators::FunctionType::HyperbolicTangent, {&Activators::Functions::HyperbolicTangent, &Activators::Derivatives::HyperbolicTangent}},
        { Activators::FunctionType::BinaryStep, {&Activators::Functions::BinaryStep, &Activators::Derivatives::BinaryStep}}
    };

/**
 *  Activator functions
 * */

double Activators::Functions::Logistic(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double Activators::Functions::HyperbolicTangent(double x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

double Activators::Functions::BinaryStep(double x) {
    return (x > 0 ? 1 : 0);
}

/**
 *  Analytical derivatives
 * */
double Activators::Derivatives::Logistic(double x) {
    double fx = Functions::Logistic(x);
    return fx * (1.0 - fx);
}

double Activators::Derivatives::HyperbolicTangent(double x) {
    double fx = Functions::HyperbolicTangent(x);
    return 1 - fx * fx;
}

double Activators::Derivatives::BinaryStep(double) {
    return 0;
}