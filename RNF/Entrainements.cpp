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
        /// propagation dans le sens classique, on garde les E
        E[0] = appliquerFonction(W[0]*Entree, activ[0], 0);
        for(unsigned n=1; n<=C; ++n){
            E[n] = appliquerFonction(W[n]*E[n-1],activ[n], 0);
        }
        vec Y = E[C];


        erreur += 0.5*(as_scalar(T-Y)*as_scalar(T-Y));

        vector<mat> V(C+1);
        ///retropropagation
        V[C] = appliquerFonction(W[C]*E[C-1], activ[C], 1)%(T-Y);
        for(unsigned n=C; n>=2; --n){
            vec K = ((W[n]).t())*V[n];
            V[n-1] = appliquerFonction(W[n-1]*E[n-2], activ[n-1], 1)%(K);
        }
        V[0] = appliquerFonction(W[0]*Entree, activ[0], 1)%(W[1].t()*V[1]);

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



//double Reseau::BFGS(Ensemble& app, double epsilon){
//    const std::vector<std::pair<arma::vec, arma::vec>>& ens = app.ens;
//
//    double erreurQuad = 0;
//
//    unsigned N = ens.size();
//    std::vector<mat> M(poids_couches.size());
//    for(mat & m : M){
//        m = eye<double>(poids_couches[]);
//    }
//
//
//    for(unsigned i = 0; i<N; ++i){ /// pour chaques exemples
//
//        auto paire = gradiants(app.ens[i]);
//        vector<mat> gradiantParCouche = paire.second;
//        vector<mat> modifCoefBiaisParCouche = paire.first;
//
//        for(unsigned j = 0; j<poids_couches.size(); ++j){
//
//
////            poids_couches[j] -= epsilon*M[j]*grad;
//        }
//
//
//    }
//
//    return erreurQuad;
//}
