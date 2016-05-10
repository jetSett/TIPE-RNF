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
                const std::vector<float>& coefBiasParCouche) :
                _entrees(entrees), _sorties(sorties), _nbCouchesCache(nbCouchesCache),
                poids_couches(structure.poids_couches), activ(structure.activ), coefBiais(structure.coefBiais){


    assert(nbNeurParCouche.size() == nbCouchesCache);
    assert(activations.size() == (nbCouchesCache+1));
    assert(coefBiasParCouche.size() == (nbCouchesCache+1));

    structure.activ = activations;

    structure.n = coefBiasParCouche.size();

    structure.tailles = std::vector<unsigned>(nbCouchesCache+1);
    structure.tailles[0] = entrees;
    for(unsigned i =0; i<nbCouchesCache; ++i){
        structure.tailles[i+1] = nbNeurParCouche[i];
    }

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

Reseau::Reseau(const std::string& file) : poids_couches(structure.poids_couches), activ(structure.activ), coefBiais(structure.coefBiais){
    load(file);
}

vec appliquerFonction(const arma::vec& e, functionDescriptor f, int derivation){
    if(f.nom == "hardlim_vect"){
        vec s = e;
        for(unsigned i = 0; i<e.size(); ++i){
            s(i) = f.func(e(i), derivation);
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
            s(i) = f.func(e(i), derivation);
        }
        return s;
    }
}

vec Reseau::resultat(const vec& e){

    return calculerSortie(structure, e);
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
    structure.tailles.resize(s+1);
    for(unsigned i = 0; i<s; ++i){
        unsigned t = 0;
        stream >> t;
        structure.tailles[i+1] = t;
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
    _entrees = poids_couches[0].n_cols;
    structure.tailles[0] = _entrees;
    structure.n = poids_couches.size();
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

void Reseau::decrire_reseau(){
    unsigned k = 0;
    for(auto m : structure.poids_couches){
        cout << k << " : " << m.n_rows << " x " << m.n_cols << endl;
        k++;
    }
    k = 0;
    for(unsigned i : structure.tailles){
        cout << k << " taille : " << i << endl;
        k++;
    }
}

ReseauStruct reseauVide(ReseauStruct& mod){
    ReseauStruct res;
    res.tailles = mod.tailles;
    res.activ = mod.activ;
    res.n = mod.n;
    res.coefBiais.resize(res.n);
    res.poids_couches.resize(res.n);
    for(unsigned k = 0; k<res.n; ++k){
        res.coefBiais[k] = vec(res.tailles[k+1], fill::zeros);
        res.poids_couches[k] = mat(res.tailles[k+1], res.tailles[k], fill::zeros);
    }
    return res;
}

void add_Reseau(ReseauStruct& r1, ReseauStruct& r2){
    assert(r1.n == r2.n);
    unsigned& n = r1.n;
    for(unsigned i = 0; i<n+1; ++i){
        assert(r1.tailles[i] == r2.tailles[i]);
    }
    for(unsigned i = 0; i<n; ++i){
        r1.coefBiais[i] += r2.coefBiais[i];
        r1.poids_couches[i] += r2.poids_couches[i];
    }
}

void mult_Reseau(ReseauStruct& res, double alpha){
    unsigned& n = res.n;

    for(unsigned i = 0; i<n; ++i){
        res.coefBiais[i] *= alpha;
        res.poids_couches[i] *= alpha;
    }
}

arma::vec calculerSortie(ReseauStruct& reseau, arma::vec E){
    assert(E.n_rows == reseau.poids_couches[0].n_cols);
    unsigned& n = reseau.n;
    std::vector<arma::vec> Y(n+1), H(n+1);
    Y[0] = E;
    H[0] = E;
    for(unsigned i=1; i<=n; ++i){
        H[i] = reseau.poids_couches[i-1]*Y[i-1] + reseau.coefBiais[i-1];
        Y[i] = appliquerFonction(H[i], reseau.activ[i-1], 0);
    }
    return Y[n];
}

ReseauStruct calculerGradient(ReseauStruct& reseau, arma::vec E, arma::vec sortieAttendue){
    assert(E.n_rows == reseau.poids_couches[0].n_cols);
    unsigned& n = reseau.n;
    std::vector<arma::vec> Y(n+1), H(n+1);
    Y[0] = E;
    H[0] = E;
    for(unsigned i=1; i<=n; ++i){
        H[i] = reseau.poids_couches[i-1]*Y[i-1] + reseau.coefBiais[i-1];
        Y[i] = appliquerFonction(H[i], reseau.activ[i-1], 0);
    }

    std::vector<arma::vec> V(n+1); // on en crée un de plus pour coller aux formules

    ReseauStruct gradient;

    gradient.n = n;
    gradient.activ = reseau.activ;
    gradient.poids_couches.resize(n);
    gradient.coefBiais.resize(n);
    gradient.tailles = reseau.tailles;

    V[n] = -2*(sortieAttendue-Y[n])%(appliquerFonction(H[n], reseau.activ[n-1], 1));
    gradient.poids_couches[n-1] = V[n]*(Y[n-1].t());
    gradient.coefBiais[n-1] = V[n];

    for(unsigned k = n-1; k >= 1; k--){
        V[k] = (reseau.poids_couches[k].t())*(V[k+1]%(appliquerFonction(H[k+1], reseau.activ[k], 1)));
        auto Hp = appliquerFonction(H[k], reseau.activ[k-1], 1);
        gradient.poids_couches[k-1] = (V[k]%Hp)*(Y[k-1].t());
        gradient.coefBiais[k-1] = V[k]%Hp;
    }

    return gradient;
}
