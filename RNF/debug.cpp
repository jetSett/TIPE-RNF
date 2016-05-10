#include <iostream>

#include "Reseau.hpp"
#include "Debug.hpp"

using namespace std;


int main(int argc, char**argv){
    Reseau res("ReseauTest");
    res.decrire_reseau();
    auto grad = calculerGradient(res.structure, arma::vec("1 2"), arma::vec("1"));
    add_Reseau(res.structure, grad);
    return 0;
}
