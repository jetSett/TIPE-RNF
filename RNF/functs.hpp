#ifndef FUNCTS_HPP_INCLUDED
#define FUNCTS_HPP_INCLUDED

#include <exception>
#include <armadillo>
#include <string>

#include "Debug.hpp"

typedef std::function<double(double, int)> fonctionActivation;

struct functionDescriptor{
    fonctionActivation func;
    std::string nom;
};

double lineaire(double e, int n);

double hardlim(double e, int n);

double hardlims(double e, int n);

double sigimoide(double e, int n);

double arctan(double e, int n);

fonctionActivation fontionFromString(std::string name);

#endif // FUNCTS_HPP_INCLUDED
