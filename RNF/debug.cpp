#include <iostream>

#include "Reseau.hpp"
#include "Debug.hpp"

using namespace std;


int main(int argc, char**argv){

    functionDescriptor lin, sig;
    lin.func = fontionFromString("lineaire");
    lin.nom = "lineaire";

    sig.func = fontionFromString("sigimoide");
    sig.nom = "sigimoide";

    Ensemble ens = Ensemble("test.txt");

    Reseau res(2, 1, 3, vector<unsigned>{5, 5, 2},
               vector<functionDescriptor>{lin, lin, lin, lin}
               ,vector<float>{0.5, 0.5, 0.5, 0.5});
//    res.decrire_reseau();
        float eps = 0.001;
        float err = res.descente_gradiant(ens, 0.001);
    for(unsigned i =0; i<1000; ++i){
//        cout << res.print_reseau() << endl<< endl;
        float err2 = res.descente_gradiant(ens, eps);
        cout << err2 << endl;
        if (abs(err2-err) <=0.01){
            eps /= 2;
        }
    }

//    cout << res.print_resultat(ens) << endl;

    return 0;
}
