#include "Reseau.hpp"

using namespace std;
using namespace arma;

double Reseau::descente_gradient(Ensemble& app, double epsilon, unsigned pas){

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

        double e = norm(T-Y)*norm(T-Y);

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

double Reseau::descente_gradient_2(Ensemble& ens, double epsilon, unsigned pas){
//    auto gradTot = reseauVide(structure);
    double epsMoy = 0;
    unsigned i = 0;
    double err = 0;
    for(auto exemple : ens.ens){
        vec sortieAtt = exemple.second;
        vec entree = exemple.first;

        vec sortie = resultat(entree);
        double e = norm(sortie - sortieAtt)*norm(sortie - sortieAtt);
        err += e;

        auto grad = calculerGradient(structure, entree, sortieAtt);
        double alpha = rechercher_alpha(epsilon, structure, grad, entree, sortieAtt, pas);
        mult_Reseau(grad, -alpha);
        add_Reseau(structure, grad);
        epsMoy = (i*epsMoy+epsilon)/(i+1);
        i++;
    }
//    Dn(epsMoy)
    return err;
}

double Reseau::gradient_conjugue(Ensemble& ens, double epsilon, unsigned cycles, unsigned pas){
    auto direction = reseauVide(structure);
    double err = 0;
    for(auto exemple : ens.ens){
        vec sortieAtt = exemple.second;
        vec entree = exemple.first;

        vec sortie = resultat(entree);
        double e = norm(sortie - sortieAtt)*norm(sortie - sortieAtt);
        err += e*e;


        ReseauStruct& X = structure;
        auto gradX = calculerGradient(X, entree, sortieAtt);
        ReseauStruct D = calculerGradient(X, entree, sortieAtt);

        double alpha = rechercher_alpha(epsilon, X, D, entree, sortieAtt, pas);

        ReseauStruct D1 = mult_Reseau_o(D, alpha);

        add_Reseau(direction, D);


        for(unsigned k =0; k<cycles; k++){

            double alpha = rechercher_alpha(epsilon, X, D, entree, sortieAtt, pas);

            D1 = mult_Reseau_o(D, alpha);

            add_Reseau(X, D1); // on a X_k+1

            auto gradXp = calculerGradient(X, entree, sortieAtt);

            float gamma = norm(calculerSortie(gradXp, entree))/norm(calculerSortie(gradX, entree));
            gamma *= gamma;

            gradX = gradXp;

            mult_Reseau(D, gamma);

            add_Reseau(D, gradXp);

        }
            add_Reseau(direction, D);

    }
    return err;
}

double Reseau::rechercher_alpha(double epsilon, ReseauStruct &res, ReseauStruct& dir, vec& entree, vec& sortieAtt, unsigned pas){
    double minEps = epsilon;
    auto d1 = mult_Reseau_o(dir, epsilon);
    ReseauStruct test = add_Reseau_o(res, d1);
    double minErr = norm(calculerSortie(test, entree)-sortieAtt);
    for(unsigned k = 0; k<pas; ++k){
        epsilon/=2;
        d1 = mult_Reseau_o(dir, epsilon);
        test = add_Reseau_o(res, d1);
        double err = norm(calculerSortie(test, entree)-sortieAtt);
        if(err < minErr and reseau_ok(test)){
            minErr = err;
            minEps = epsilon;
        }
    }
    return minEps;
}
