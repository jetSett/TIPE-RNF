#include "Reseau.hpp"

#include "Debug.hpp"

using namespace std;
using namespace arma;

void Ensemble::load(const std::string& file){
    int tailleEntree = 0, tailleSortie = 0;
    ifstream stream(file);
    stream >> tailleEntree;
    stream >> tailleSortie;
    int nombreApprentissage = 0;
    stream >> nombreApprentissage;
    ens.resize(nombreApprentissage);
    for(int i = 0; i<nombreApprentissage; i++){
        vec entree = vec(tailleEntree);
        for(int j = 0; j<tailleEntree; ++j){
            stream >> entree(j);
        }
        vec sortie = vec(tailleSortie);
        for(int j = 0; j<tailleSortie; ++j){
            stream >> sortie(j);
        }
        ens[i] = make_pair(entree, sortie);
    }
}

std::string Ensemble::print_apprentiss(){
    std::ostringstream oss;
    for(auto a : ens){
        oss << a.first << std::endl << "Donne" << a.second << "\n";
    }
    return oss.str();
}

Reseau::Reseau(unsigned entrees, unsigned sorties, unsigned nbCouchesCache,
                const std::vector<unsigned>& nbNeurParCouche,
                const std::vector<functionDescriptor>& activations,
                const std::vector<float>& coefBiasParCouche) : activ(activations),
                _entrees(entrees), _sorties(sorties), _nbCouchesCache(nbCouchesCache){


    assert(nbNeurParCouche.size() == nbCouchesCache);
    assert(activations.size() == (nbCouchesCache+1));
    assert(coefBiasParCouche.size() == (nbCouchesCache+1));



    poids_couches.resize(nbCouchesCache+1);
    coefBiais.resize(nbCouchesCache+1);
    if(nbCouchesCache == 0){
        poids_couches[0] = mat(sorties, entrees, fill::randu);
        coefBiais[0] = vec(sorties);
        coefBiais[0].fill(coefBiasParCouche[0]);
    }else{
        poids_couches[0] = mat(nbNeurParCouche[0], entrees, fill::randu);
        coefBiais[0] = vec(nbNeurParCouche[0]);
        coefBiais[0].fill(coefBiasParCouche[0]);
        for(unsigned i = 0; i<nbCouchesCache-1; ++i){
            poids_couches[i+1] = mat(nbNeurParCouche[i+1], nbNeurParCouche[i], fill::randu);
            coefBiais[i+1] = vec(nbNeurParCouche[i+1]);
            coefBiais[i+1].fill(coefBiasParCouche[i+1]);
        }
        poids_couches[poids_couches.size()-1] = mat(sorties, nbNeurParCouche[nbCouchesCache-1], fill::randu);
        coefBiais[poids_couches.size()-1] = vec(sorties);
        coefBiais[poids_couches.size()-1].fill(coefBiasParCouche[poids_couches.size()-1]);
    }
}

Reseau::Reseau(const std::string& file){
    load(file);
}

vec Reseau::appliquerFonction(const arma::vec& e, functionDescriptor f, int n){
    if(f.nom == "hardlim_vect"){
        vec s = e;
        for(unsigned i = 0; i<e.size(); ++i){
            s(i) = f.func(e(i), n);
        }
        double m = max(s);
        bool c = false;
        for(unsigned i = 0; i<e.size(); ++i){
            if(s(i) == m and !c){
                s(i) = s(i)==m;
                c = true;
            }else{
                s(i) = 0;
            }
        }
        return s;
    }else{
        vec s = e;
        for(unsigned i = 0; i<e.size(); ++i){
            s(i) = f.func(e(i), n);
        }
        return s;
    }
}

vec Reseau::resultat(const vec& e){
    vec s = e;
    for(unsigned i = 0; i<poids_couches.size(); ++i){
        s = appliquerFonction(poids_couches[i]*s, activ[i], 0);
    }
    return s;
}

double Reseau::verification(const Ensemble& ens){
    double erreurQuad = 0;
    for(auto p : ens.ens){
        vec res = resultat(p.first);
        double erreur = accu(p.second - res);
        erreurQuad += erreur*erreur;
    }
    return std::sqrt(erreurQuad);
}

void Reseau::save(const std::string& file){
    ofstream stream(file);
    stream << coefBiais.size() << endl;
    for(unsigned i = 0; i < coefBiais.size(); ++i){
        stream << coefBiais[i].size() << endl;
        for(unsigned j = 0; j<coefBiais[i].size(); ++j){
            stream << coefBiais[i](j) << " ";
        }
        stream << endl;
    }
    std::string nom = "_mat_"+file;
    stream << poids_couches.size() << endl;
    for(unsigned i = 0; i<poids_couches.size(); ++i){
        ostringstream oss;
        oss << i;
        oss << nom;
        stream << oss.str() << endl;
        poids_couches[i].save(oss.str());
        oss.clear();
    }
    for(unsigned i = 0; i<poids_couches.size(); ++i){
        stream << activ[i].nom << endl;
    }
}

void Reseau::load(const std::string& file){
    ifstream stream(file);
    assert(stream);
    unsigned s = 0;
    stream >> s;
    coefBiais.resize(s);
    for(unsigned i = 0; i<s; ++i){
        unsigned t = 0;
        stream >> t;
        coefBiais[i] = vec(t);
        for(unsigned j = 0; j<t; ++j){
            stream >> coefBiais[i](j);
        }
    }
    stream >> s;
    poids_couches.resize(s);
    for(unsigned i = 0; i<s; ++i){
        std::string s;
        stream >> s;
        poids_couches[i].load(s);
    }
    activ.resize(poids_couches.size());
    for(unsigned i = 0; i<poids_couches.size(); ++i){
        functionDescriptor f;
        stream >> f.nom;
        f.func = fontionFromString(f.nom);
        activ[i] = f;
    }

    _nbCouchesCache = poids_couches.size() - 1;
    _entrees = poids_couches[0].n_rows;
    _sorties = poids_couches[poids_couches.size()-1].n_rows;
}

std::string Reseau::print_reseau(){
    std::ostringstream oss;
    int i = 0;
    for(auto a : poids_couches){
        oss << "Couche n" << i << std::endl;
        oss << a << std::endl;
        oss << "Coef de biais" << std::endl;
        oss << coefBiais[i] << std::endl;
        oss << "Fonction d'activation" << std::endl;
        oss << activ[i].nom << std::endl;
        ++i;
    }
    return oss.str();
}

std::string Reseau::print_resultat(const Ensemble& ens){
    std::ostringstream oss;
    for(auto a : ens.ens){
        oss << a.first << " Donne " << std::endl;
        oss << resultat(a.first) << std::endl;
    }
    return oss.str();
}


//std::pair<std::vector<arma::mat>, std::vector<arma::mat>> Reseau::gradiants(std::pair<arma::vec, arma::vec> in_out){
//        std::pair<std::vector<arma::mat>, std::vector<arma::mat>> paire; //en first les coefs de biais, en second les gradiants
//        int M = poids_couches.size();
//        vector<vec> a(M+1);
//        a[0] = in_out.first;
//        for(int k = 1; k<=M; ++k){
//            a[k] = appliquerFonction(poids_couches[k-1]*a[k-1]-coefBiais[k-1], activ[k-1], 0);
//        } ///Aller
//
//        vector<mat> s(M);
//
//        s[M-1] = -2*fPoint(coefBiais[M-1], activ[M-1])*coefBiais[M-1]*(in_out.second-a[M]);
//        for(int k = M-2; k>=0; --k){
//            s[k] = fPoint(coefBiais[k], activ[k])*poids_couches[k+1].t()*s[k+1];
//        } ///Retour
//        paire.first.resize(coefBiais.size());
//        paire.second.resize(poids_couches.size());
//        for(int k = 0; k<M; ++k){
//            paire.second[k] = s[k]*(a[k].t());
//            paire.first[k] = s[k];
//        }
//        return paire;
//}

void Reseau::decrire_reseau(){
    unsigned k = 0;
    for(auto m : poids_couches){
        cout << k << " : " << m.n_rows << " x " << m.n_cols << endl;
        k++;
    }
}
