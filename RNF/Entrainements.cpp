#include "Reseau.hpp"

using namespace std;
using namespace arma;

double Reseau::descente_gradiant_1(Ensemble& app, double epsilon){
    const std::vector<std::pair<arma::vec, arma::vec>>& ens = app.ens;
    int M = poids_couches.size();

    double erreurQuad = 0; /// Calcul de l'erreur quadratique

    for(unsigned p = 0; p<ens.size(); ++p){
        vector<vec> a(M+1);
        a[0] = ens[p].first;
        for(int k = 1; k<=M; ++k){
            a[k] = appliquerFonction(poids_couches[k-1]*a[k-1]-coefBiais[k-1], activ[k-1], 0);
        } ///Aller

        vector<mat> e(M);

        double erreur = as_scalar(ens[p].second-a[M]);

        erreurQuad += erreur*erreur;

        e[M-1] = -2*fPoint(coefBiais[M-1], activ[M-1])*coefBiais[M-1]*(ens[p].second-a[M]);
        for(int k = M-2; k>=0; --k){
            e[k] = fPoint(coefBiais[k], activ[k])*poids_couches[k+1].t()*e[k+1];
        } ///Retour

        for(int k = 0; k<M; ++k){
            poids_couches[k] += -epsilon*e[k]*(a[k].t());
            coefBiais[k] += epsilon*e[k];
        } ///On actualise
    }

    return std::sqrt(erreurQuad);
}


double Reseau::descente_gradiant_2(Ensemble& app, double epsilon){
    const std::vector<std::pair<arma::vec, arma::vec>>& ens = app.ens;
    int M = poids_couches.size();

    double erreurQuad = 0; /// Calcul de l'erreur quadratique

    for(unsigned p = 0; p<ens.size(); ++p){
        vector<vec> a(M+1);
        vector<vec> h(M);
        a[0] = ens[p].first;
        for(int k = 1; k<=M; ++k){
            h[k-1] = poids_couches[k-1]*a[k-1]-coefBiais[k-1];


            a[k] = appliquerFonction(h[k-1], activ[k-1], 0);
        } ///Aller

        vector<mat> e(M);

        double erreur = as_scalar(ens[p].second-a[M]);

        erreurQuad += erreur*erreur;

        vec y = a[M];
        vec t = ens[p].second;

        e[M-1] = fPoint(h[M-1], activ[M-1])*(t-y);
        for(int k = M-2; k>=0; --k){

            mat alpha = poids_couches[k+1].t()*e[k+1];

            e[k] = fPoint(h[k+1], activ[k])%alpha;
        } ///Retour

        for(int k = 0; k<M; ++k){
            mat deltaW = poids_couches[k];

            for(int i = 0; i < deltaW.n_rows; ++i){
                for(int j = 0; j< deltaW.n_cols; ++j){
                    deltaW(i, j) = e[k][i]*a[k][j];
                }
            }

            poids_couches[k] += -epsilon*deltaW;
            coefBiais[k] += epsilon*e[k]; /// on le garde dans le doute
        } ///On actualise
    }

    return std::sqrt(erreurQuad);
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
