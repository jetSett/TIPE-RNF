#include <random>
#include "Application.hpp"

namespace po = boost::program_options;

Application::Application()
{
    arma::arma_rng::set_seed_random();
    _descriptor.add_options()
        ("help", "Afficher la liste des options")
        ("create", "Créer un nouveau RNF")
        ("learn", "Faire apprendre un RNF")
        ("res", "Donner le résultat d'un RNF")
        ("reseau,r", po::value<std::string>(), "Choix du fichier de RNF (à créer ou à charger)");


    po::options_description create("Paramètres de création");
    create.add_options()
        ("nbEntree", po::value<unsigned>(), "Taille de l'entrée")
        ("sorties,s", po::value<unsigned>(), "Taille de la sortie")
        ("nbCouchesCache,c", po::value<unsigned>(), "Nombre de couches caches")

        ("nbNeurParCouche,n", po::value<std::vector<unsigned>>()->multitoken(),
         "Nombre de neurones par couches cachées (ne pas inclure les neurones d'entrées)")

        ("fonctions,f", po::value<std::vector<std::string>>()->multitoken(),
         "Fonctions d'activations du neurone")

        ("coefBiais,b", po::value<std::vector<float>>()->multitoken(),
         "Coef de biais par couches (inclure la couche d'entrée) si non entrés, tirés au hasard [TODO]");

    po::options_description learning("Paramètres d'apprentissage");
    learning.add_options()
        ("methode,m", po::value<std::string>()->default_value("gradDesc"), "Choix de la méthode utilisée")
        ("epsilon", po::value<float>(), "Choix de la constante d'apprentissage")
        ("apprentissage,e", po::value<std::string>(), "Choix du fichier d'apprentissage");

    po::options_description result("Paramètres de résultat");
    result.add_options()
        ("entree,i", po::value<std::vector<float>>()->multitoken(), "Choix de l'entrée");

    _descriptor.add(create).add(learning).add(result);

}

Application::~Application()
{
}

void Application::run(int argc, char** argv){
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, _descriptor), vm);
    po::notify(vm);
    auto engine = std::default_random_engine{};
    if(vm.count("help")){ //help
        std::cout << _descriptor << std::endl;
    }else if(vm.count("reseau")){
        if(vm.count("create")){ //creation

            unsigned entrees = vm["nbEntree"].as<unsigned>();
            unsigned sorties = vm["sorties"].as<unsigned>();
            unsigned nbCoucheCache =vm["nbCouchesCache"].as<unsigned>();
            std::vector<unsigned> nbNeurPCouche = nbCoucheCache==0?std::vector<unsigned>(): vm["nbNeurParCouche"].as<std::vector<unsigned>>();

            std::vector<functionDescriptor> fontions; // on relie les fonctions

            for(std::string s : vm["fonctions"].as<std::vector<std::string>>()){
                functionDescriptor f;
                f.func = fontionFromString(s);
                f.nom = s;
                fontions.push_back(f);
            }

            std::vector<float> coefBiais(0);

            if(vm.count("coefBiais"))
                coefBiais = vm["coefBiais"].as<std::vector<float>>();

            Reseau r(entrees, sorties, nbCoucheCache, nbNeurPCouche, fontions, coefBiais);
            r.print_reseau();

            r.save(vm["reseau"].as<std::string>());

            }else{ //on doit charger un réseau
                    Reseau r(vm["reseau"].as<std::string>());
                    if(vm.count("learn")){ //apprentissage
                        std::string methode = vm["methode"].as<std::string>();
                        Ensemble app;
                        app.load(vm["entree"].as<std::string>());

                        if(methode == "gradDesc"){
                            r.descente_gradient(app, vm["epsilon"].as<float>(), 0);
                        }else{
                            std::cerr << "Merci de préciser une methode valable" << std::endl;
                        }
                        r.save(vm["reseau"].as<std::string>());

                    }else if(vm.count("res")){ //resultat
                        auto entreeVect = vm["entree"].as<std::vector<float>>();
                        arma::vec entree(entreeVect.size());

                        unsigned i = 0;
                        for(float a : entreeVect){
                            entree(i) = a;
                            ++i;
                        }

                        std::cout << r.resultat(entree) << std::endl;
                    }else{
                        std::cerr << "Erreur, merci de préciser un mode de fonctionnement" << std::endl;
                        return;
                    }

            }
        }else{
                std::cerr << "Erreur, merci de préciser un réseau" << std::endl;
        }
}
