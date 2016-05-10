#ifndef HOPFIELD_HPP
#define HOPFIELD_HPP

#include <cstdlib>
#include <ctime>

#include <iostream>

#include <vector>
#include <armadillo>

struct EnsembleHopfield{
    EnsembleHopfield(const std::vector<arma::vec>& e) : ensembles(e){}
    std::vector<arma::vec> ensembles;
};

class Hopfield
{
    public:
        Hopfield(EnsembleHopfield& ens);

        arma::vec result(arma::vec entree, unsigned iter);

    private:
        void enseigner(EnsembleHopfield& ens);

        unsigned int n, patterns;
        arma::mat _W;
        arma::vec _neurones;
};

#endif // HOPFIELD_HPP
