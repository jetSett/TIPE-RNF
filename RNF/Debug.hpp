#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

#include <iostream>
#include <vector>

#define D(a) std::cout << (a) << std::endl;
#define Dn(a) std::cout << #a << std::endl << a << std::endl;

template<class A>
void afficherVector(const std::vector<A>& e){
    for(A a : e)
        D(a)
}

#endif // DEBUG_HPP_INCLUDED
