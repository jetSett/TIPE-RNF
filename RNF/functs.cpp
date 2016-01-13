#include <map>

#include "functs.hpp"

fonctionActivation fontionFromString(std::string name){
    std::map<std::string, fonctionActivation> corresp;
    corresp["lineaire"] = lineaire;
    corresp["hardlim"] = hardlim;
    corresp["hardlims"] = hardlims;
    corresp["sigimoide"] = sigimoide;
    corresp["arctan"] = arctan;
    corresp["hardlim_vect"] = hardlim;

    return corresp[name];
}



/// /!\ n = 1 -> dérivée première

double lineaire(double e, int n){
    return n==1?1:e;
}

double hardlim(double e, int n){
    if(n>=1) ///dérivée premiere
        return 1;
    return e<0?0:1;
}

double hardlims(double e, int n){
    if(n>=1) ///dérivée premiere
        return 1;
    return e<0?-1:1;
}

double sigimoide(double e, int n){
    if(n == 1){
        float a = exp(-e);
        return a/((1+a)*(1+a));
    }else{
        return (1/(1+exp(-e)));
    }
}

double arctan(double e, int n){
    return n==1?(1/(1+e*e)):atan(e);
}
