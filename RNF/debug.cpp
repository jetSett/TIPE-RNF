#include <iostream>
#include <fstream>

#include "Reseau.hpp"
#include "Debug.hpp"

#include "Chrono.hpp"

using namespace std;


int main(int argc, char**argv){

    ofstream of("Result.xls");

    of << "Neurones, T (ms)" << endl;

    Ensemble ens(std::vector<std::pair<arma::vec, arma::vec>>(2000, std::pair<arma::vec, arma::vec>(arma::vec("2 2 0"), arma::vec("0"))));

    for(auto &a : ens.ens){
        a = std::pair<arma::vec, arma::vec>(arma::vec(3, arma::fill::randu), arma::vec(1, arma::fill::randu));
    }

    for(unsigned N = 2; N<5000; N*=2){
        Reseau res(3, 1, 1, std::vector<unsigned>({N}), std::vector<functionDescriptor>({functionDescriptor("sigimoide"), functionDescriptor("hardlim")}),
                   std::vector<float>({0.5, 0.5}));
        JetChrono::Chrono<std::milli, unsigned long> c(false);
        cout << res.descente_gradient_2(ens, 0.1, 0) << endl;
        unsigned long t = c.elapsedTime().count();
        cout << N << "," << t << "\n";
        of << N << "," << t << "\n";
    }
    return 0;
}
