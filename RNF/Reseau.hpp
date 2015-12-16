#ifndef RESEAU_HPP_INCLUDED
#define RESEAU_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <fstream>
#include <sstream>

#include <tuple>

#include <armadillo>

#include "functs.hpp"

struct Ensemble{
    std::vector<std::pair<arma::vec, arma::vec>> ens;

    std::string print_apprentiss();

    Ensemble(){}
    Ensemble(const std::string& file){
        load(file);
    }

    void shuffle(){
        auto engine = std::default_random_engine{};

        std::shuffle(std::begin(ens), std::end(ens), engine);
    }

    int taille(){
        return ens.size();
    }

    void load(const std::string& file);
};

class Reseau{
public:
    Reseau(unsigned entrees, unsigned sorties, unsigned nbCouchesCache,
           const std::vector<unsigned>& nbNeurParCouche,
           const std::vector<functionDescriptor>& activations,
           const std::vector<float>& coefBiasParCouche);

    Reseau(const std::string& file);

    arma::vec resultat(const arma::vec& e);

    arma::vec appliquerFonction(const arma::vec& e, functionDescriptor f, int n);

    void save(const std::string& file);
    void load(const std::string& file);

    std::string print_resultat(const Ensemble& ens);

    std::string print_reseau();
    void decrire_reseau();

    std::pair<std::vector<arma::mat>, std::vector<arma::mat>> gradiants(std::pair<arma::vec, arma::vec> in_out); //en first les coefs de biais, en second les gradiants
    double verification(const Ensemble& ens);

    double descente_gradiant(Ensemble& app, double epsilon);

//private:
    std::vector<functionDescriptor> activ;

    std::vector<arma::vec> coefBiais;

    std::vector<arma::mat> poids_couches;

    unsigned _entrees, _sorties, _nbCouchesCache;
};

#endif // RESEAU_HPP_INCLUDED
