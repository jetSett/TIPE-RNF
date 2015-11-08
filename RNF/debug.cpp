#include <iostream>

#include "Reseau.hpp"

using namespace std;

int main(int argc, char**argv){
    Reseau res("TestXOR");
    Ensemble ens("/home/joel/Prog/TIPE/RNF/ensembleXOR.txt");

    double erreur = 1000;

    while(erreur > 1){
        erreur = res.descente_gradiant_2(ens, 0.1);
        cout << erreur << endl;
    }

    cout << "Verification" << endl;

    cout << res.resultat(arma::vec({0, 0})) << endl;

    cout << res.verification(ens) << endl;
    cout << "Fin" << endl;

    return 0;
}
