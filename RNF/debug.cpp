#include <iostream>

#include "Reseau.hpp"
#include "Debug.hpp"

using namespace std;


int main(int argc, char**argv){

    functionDescriptor lin;
    lin.func = fontionFromString("lineaire");
    lin.nom = "lineaire";

    Ensemble ens = Ensemble("test.txt");

    Reseau res(2, 1, 2, vector<unsigned>{10, 3},
               vector<functionDescriptor>{lin, lin, lin}
               ,vector<float>{0.5, 0.5, 0.5});

    for(unsigned i =0; i<100; ++i){
//        cout << res.print_reseau() << endl<< endl;
        cout << res.descente_gradiant(ens, 0.001) << endl;
    }

    cout << res.print_resultat(ens) << endl;

    return 0;
}
