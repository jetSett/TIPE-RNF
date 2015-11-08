#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <random>

#include <boost/program_options.hpp>

#include <armadillo>

#include "functs.hpp"
#include "Reseau.hpp"

class Application
{
    public:
        Application();
        ~Application();

        void run(int argc, char** argv);

    private:
        boost::program_options::options_description _descriptor;
};

#endif // APPLICATION_H
