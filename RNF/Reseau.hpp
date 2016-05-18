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

    Ensemble(std::vector<std::pair<arma::vec, arma::vec>> e) : ens(e){}
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

struct ReseauStruct{
    unsigned n;

    std::vector<unsigned> tailles; //de taille n+1 !

    std::vector<arma::vec> coefBiais; // de taille n

    std::vector<arma::mat> poids_couches; // de taille n

    std::vector<functionDescriptor> activ; // de taille n
};

bool reseau_ok(ReseauStruct& r);

arma::vec appliquerFonction(const arma::vec& e, functionDescriptor f, int derivation);

void add_Reseau(ReseauStruct& r1, ReseauStruct& r2);

ReseauStruct add_Reseau_o(ReseauStruct& r1, ReseauStruct& r2);

void mult_Reseau(ReseauStruct& r, double alpha);

ReseauStruct mult_Reseau_o(ReseauStruct& r, double alpha);

ReseauStruct reseauVide(ReseauStruct& r);

arma::vec calculerSortie(ReseauStruct& reseau, arma::vec E);

ReseauStruct calculerGradient(ReseauStruct& reseau, arma::vec E, arma::vec sortieAttendue);

class Reseau{
public:
    Reseau(unsigned entrees, unsigned sorties, unsigned nbCouchesCache,
           const std::vector<unsigned>& nbNeurParCouche,
           const std::vector<functionDescriptor>& activations,
           const std::vector<float>& coefBiasParCouche);

    Reseau(const std::string& file);

    arma::vec resultat(const arma::vec& e);

    void save(const std::string& file);
    void load(const std::string& file);

    std::string print_resultat(const Ensemble& ens);

    std::string print_reseau();
    void decrire_reseau();

    double verification(const Ensemble& ens);

    double descente_gradient(Ensemble& app, double epsilon, unsigned pas);

    double descente_gradient_2(Ensemble& app, double epsilon, unsigned pas);

    double gradient_conjugue(Ensemble& ens, double epsilon, unsigned cycles, unsigned pas);

    /// cherche le alpha qui va minimiser F(res + alpha*dir)
    double rechercher_alpha(double epsilon, ReseauStruct& res, ReseauStruct& dir, arma::vec& entree, arma::vec& sortieAtt, unsigned pas);

    bool systeme_ok(){
        return reseau_ok(structure);
    }

//private:
    std::vector<functionDescriptor>& activ;

    std::vector<arma::vec>& coefBiais;

    std::vector<arma::mat>& poids_couches;

    ReseauStruct structure;

    unsigned _entrees, _sorties, _nbCouchesCache;
};



#endif // RESEAU_HPP_INCLUDED
