#include "Reseau.hpp"

using namespace std;
using namespace arma;

double Reseau::descente_gradient(Ensemble& app, double epsilon){

    const std::vector<std::pair<arma::vec, arma::vec>>& ens = app.ens;
    unsigned int C = poids_couches.size()-1;

    auto& W = poids_couches;

    unsigned int S = _sorties, E = _entrees;

    double erreur = 0; /// Calcul de l'erreur
    double modification = 0, modificationAbs = 0;

    for(unsigned p = 0; p<ens.size(); ++p){
        vec Entree = ens[p].first;
        vec T = ens[p].second;

        vector<mat> E(C+1);

        vector<mat> H(C+1);
        /// propagation dans le sens classique, on garde les E
        H[0] = W[0]*Entree+coefBiais[0];
        E[0] = appliquerFonction(H[0], activ[0], 0);
        for(unsigned n=1; n<=C-1; ++n){
            H[n] = W[n]*E[n-1]+coefBiais[n];
            E[n] = appliquerFonction(H[n],activ[n], 0);
        }
        H[C] = W[C]*E[C-1]+coefBiais[C];
        E[C] = appliquerFonction(H[C],activ[C], 0);
        vec Y = E[C];

        double e = accu(abs(T-Y));

        erreur += 0.5*(e*e);

        vector<mat> V(C+1);

        coefBiais[C] += epsilon*(T-Y);

        ///retropropagation
        V[C] = appliquerFonction(W[C]*E[C-1], activ[C], 1)%(T-Y);
        for(unsigned n=C; n>=1; --n){
            vec K = ((W[n]).t())*V[n];
            V[n-1] = appliquerFonction(H[n-1], activ[n-1], 1)%(K);
            coefBiais[n-1] += epsilon*K;
        }

        /// mise à jour des poids

        for(unsigned i = 0; i<W[0].n_rows; ++i){
            for(unsigned j = 0; j<W[0].n_cols; ++j){
                W[0](i, j) += epsilon*V[0](i)*Entree(j);
                modification += epsilon*V[0](i)*Entree(j);
                modificationAbs += abs(epsilon*V[0](i)*Entree(j));
            }
        }

        for(unsigned n = 1; n<=C; ++n){
            for(unsigned i = 0; i<W[n].n_rows; ++i){
                for(unsigned j = 0; j<W[n].n_cols; ++j){
                    W[n](i, j) += epsilon*V[n](i)*E[n-1](j);
                    modification += epsilon*V[n](i)*E[n-1](j);
                    modificationAbs += abs(epsilon*V[n](i)*E[n-1](j));
                }
            }
        }
    }

//    Dn(modification)
//    Dn(modificationAbs)

    return erreur;
}

double Reseau::descente_gradient_2(Ensemble& ens, double epsilon){
    auto gradTot = reseauVide(structure);
    double err = 0;
    for(auto exemple : ens.ens){
        vec sortieAtt = exemple.second;
        vec entree = exemple.first;

        vec sortie = resultat(entree);
        double e = accu(sortie - sortieAtt);
        err += e*e;

        auto grad = calculerGradient(structure, entree, sortieAtt);

        add_Reseau(gradTot, grad);
    }
    mult_Reseau(gradTot, -epsilon);
    add_Reseau(structure, gradTot);
    return err;
}

double Reseau::gradient_conjugue(Ensemble& ens, double epsilon, unsigned cycles){
    auto direction = reseauVide(structure);
    double err = 0;
    for(auto exemple : ens.ens){
        vec sortieAtt = exemple.second;
        vec entree = exemple.first;

        vec sortie = resultat(entree);
        double e = sum(sortie - sortieAtt);
        err += e*e;


        ReseauStruct X = structure;
        auto gradX = calculerGradient(X, entree, sortieAtt);
        ReseauStruct D = calculerGradient(X, entree, sortieAtt);
        mult_Reseau(D, 0.5);
        add_Reseau(direction, D);

        for(unsigned k =0; k<cycles; k++){

            mult_Reseau(D, -epsilon);
            add_Reseau(X, D); // on a X_k+1
            mult_Reseau(D, -1/epsilon);

            auto gradXp =calculerGradient(X, entree, sortieAtt);

            float gamma = norm(calculerSortie(gradXp, entree))/norm(calculerSortie(gradX, entree));
            gamma *= gamma;

            gradX = gradXp;

            mult_Reseau(gradX, 0.5);
            mult_Reseau(D, gamma);
            mult_Reseau(gradX, 2);

            add_Reseau(D, gradXp);

            add_Reseau(direction, D);
        }

    }
    mult_Reseau(direction, -epsilon);
    add_Reseau(structure, direction);
    return err;
}
