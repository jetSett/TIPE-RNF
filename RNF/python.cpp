#include <Python.h>
#include <boost/python.hpp>

#include <sstream>

#include "Reseau.hpp"
#include "Hopfield.hpp"

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

    class_<Ensemble>("Ensemble", init<std::vector<std::pair<arma::vec, arma::vec>>>())
    .def(init<const std::string&>())
    .def("__str__", &Ensemble::print_apprentiss)
    .def("shuffle", &Ensemble::shuffle)
    .def("load", &Ensemble::load)
    .def_readwrite("ens", &Ensemble::ens)
    .def("__del__", emptyFunc<Ensemble>)
    .def("taille", &Ensemble::taille)
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
    .def("decrire_reseau", &Reseau::decrire_reseau)
    .def("resultat", &Reseau::resultat)
    .def("apprentGrad", &Reseau::descente_gradient)
    .def("apprentGrad2", &Reseau::descente_gradient_2)
    .def("gradConjugue", &Reseau::gradient_conjugue)
    .def("save", &Reseau::save)
    .def("__str__", &Reseau::print_reseau)
    .def("__del__", emptyFunc<Reseau>)
    .def("print_resultat", &Reseau::print_resultat)
    .def("verification", &Reseau::verification)
    .def("is_ok", &Reseau::systeme_ok)
    ;

    class_<EnsembleHopfield>("EnsembleHopfield", init<const std::vector<arma::vec>&>())
    .def("__del__", emptyFunc<EnsembleHopfield>);
    ;

    class_<Hopfield>("Hopfield", init<EnsembleHopfield&>())
    .def("result", &Hopfield::result)
    .def("__del__", emptyFunc<Hopfield>)
    ;

    class_<functionDescriptor>("functionDescriptor", init<const std::string&>())
    .def("__del__", emptyFunc<functionDescriptor>)
    ;
}
