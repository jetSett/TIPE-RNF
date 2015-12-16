#include "Reseau.hpp"

using namespace std;
using namespace arma;

double Reseau::descente_gradiant(Ensemble& app, double epsilon){

    const std::vector<std::pair<arma::vec, arma::vec>>& ens = app.ens;
    unsigned int C = poids_couches.size()-1;

    auto& W = poids_couches;

    unsigned int S = _sorties, E = _entrees;

    double erreur = 0; /// Calcul de l'erreur

    for(unsigned p = 0; p<ens.size(); ++p){
        vec Entree = ens[p].first;
        vec T = ens[p].second;

        vector<mat> E(C+1);

        vector<mat> H(C+1);
        /// propagation dans le sens classique, on garde les E
        H[0] = W[0]*Entree;
        E[0] = appliquerFonction(H[0], activ[0], 0);
        for(unsigned n=1; n<=C; ++n){
            H[n] = W[n]*E[n-1];
            E[n] = appliquerFonction(H[n],activ[n], 0);
        }
        vec Y = E[C];

        double e = accu(abs(T-Y));

        erreur += 0.5*(e*e);

        vector<mat> V(C+1);
        ///retropropagation
        V[C] = appliquerFonction(W[C]*E[C-1], activ[C], 1)%(T-Y);
        for(unsigned n=C; n>=2; --n){
            vec K = ((W[n]).t())*V[n];
            V[n-1] = appliquerFonction(H[n-1], activ[n-1], 1)%(K);
        }
        V[0] = appliquerFonction(H[0], activ[0], 1)%(W[1].t()*V[1]);

        /// mise à jour des poids

        for(unsigned i = 0; i<W[0].n_rows; ++i){
            for(unsigned j = 0; j<W[0].n_cols; ++j){
                W[0](i, j) += epsilon*V[0](i)*Entree(j);
            }
        }

        for(unsigned n = 1; n<=C; ++n){
            for(unsigned i = 0; i<W[n].n_rows; ++i){
                for(unsigned j = 0; j<W[n].n_cols; ++j){
                    W[n](i, j) += epsilon*V[n](i)*E[n-1](j);
                }
            }
        }
    }

    return erreur;
}
