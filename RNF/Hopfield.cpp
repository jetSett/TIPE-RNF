#include "Hopfield.hpp"

using namespace std;
using namespace arma;

Hopfield::Hopfield(EnsembleHopfield& ens)
{
    patterns = ens.ensembles.size();
    n = ens.ensembles[0].n_rows;
    for(unsigned i = 0; i<patterns; ++i){
        if(ens.ensembles[i].n_rows != n){
            cerr << i << " eme exemple de taille diff de " << n << endl;
            exit(EXIT_FAILURE);
        }
    }
    srand(time(nullptr));

    _W = mat(n, n, fill::zeros);
    _neurones = vec(n, fill::zeros);

    enseigner(ens);
}

void Hopfield::enseigner(EnsembleHopfield& ens){
    for(unsigned i=0; i<n; ++i){
        for(unsigned j=0; j<n; ++j){
            if(j!=i){
                float s = 0.0;
                for(unsigned k = 0; k<patterns; ++k){
                    s += ens.ensembles[k](i)*ens.ensembles[k](j);
                }
                _W(i, j) = s/patterns;
            }
        }
    }
}

arma::vec Hopfield::result(arma::vec entree, unsigned iter){
    _neurones = entree;
    for(unsigned i=0; i<iter; ++i){
        unsigned j = rand()%n;
        float s = 0.0;
        for(unsigned k = 0; k<n; ++k){
            s += _W(j, k)*_neurones[k];
        }
        _neurones(j)  = s>0?1:0;
    }
    return _neurones;
}
