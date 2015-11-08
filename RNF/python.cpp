#include <Python.h>
#include <boost/python.hpp>

#include <sstream>

#include "Reseau.hpp"

using namespace boost::python;

std::string afficherArmaVec(const arma::vec& v){
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

template<class T>
void emptyFunc(const T&){
}

BOOST_PYTHON_MODULE(RNF)
{

    class_<Ensemble>("Ensemble")
    .def(init<const std::string&>())
    .def("__str__", &Ensemble::print_apprentiss)
    .def("shuffle", &Ensemble::shuffle)
    .def("load", &Ensemble::load)
    .def_readwrite("ens", &Ensemble::ens)
    .def("__del__", emptyFunc<Ensemble>)
    ;

    class_<arma::vec>("vec", init<std::string>())
    .def("__str__", afficherArmaVec)
    .def("__del__", emptyFunc<arma::vec>)
    ;

    class_<Reseau>("Reseau", init<const std::string&>())
    .def( init<unsigned, unsigned, unsigned,
           const std::vector<unsigned>&,
           const std::vector<functionDescriptor>&,
           const std::vector<float>& >() )
    .def("resultat", &Reseau::resultat)
    .def("apprentGrad1", &Reseau::descente_gradiant_1)
    .def("apprentGrad2", &Reseau::descente_gradiant_2)
//    .def("apprentBFGS", &Reseau::BFGS)
    .def("save", &Reseau::save)
    .def("__str__", &Reseau::print_reseau)
    .def("__del__", emptyFunc<Reseau>)
    .def("print_resultat", &Reseau::print_resultat)
    .def("verification", &Reseau::verification)
    ;
}
