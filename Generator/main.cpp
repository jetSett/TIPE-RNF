#include <iostream>
#include <fstream>

#include <2D/Renderer.hpp>
#include <2D/Texture.hpp>
#include <Graphics/WindowsManager.hpp>
#include <2D/TileMap.hpp>

#define D(a) std::cout << (a) << std::endl;
#define Dn(a) std::cout << #a << " : " << a << std::endl;
#define Dn2(a) std::cout << #a << "\n" << a << std::endl;
#define LINE() std::cout << "\n";

using namespace std;

using namespace Graphics;

using namespace D2;

string fichierOut = "out.exemples";

const int LARGEUR = 50;
const int HAUTEUR = 50;

const int tailleBrosse = 2;

/// Nombre total d'exemples : LARGEUR*HAUTEUR

const int LARGEUR_EFF = 10;
const int HAUTEUR_EFF = 10;

const float pas_l = (float)LARGEUR_EFF/(float)LARGEUR;
const float pas_h = (float)HAUTEUR_EFF/(float)HAUTEUR;

const int PIX_TAILLE = 15;

Window2DPtr win = nullptr;

const int Finput = 0;


Image alive(glm::ivec2(PIX_TAILLE, PIX_TAILLE), PXF_A8R8G8B8), dead(glm::ivec2(PIX_TAILLE, PIX_TAILLE), PXF_R8G8B8);


typedef vector<vector<int>> grille;

grille grid(HAUTEUR, vector<int>(LARGEUR, 0));

void clearGrid(){
    for(auto& a : grid){
        for(int& b : a){
            b = 0;
        }
    }
}

ShapePtr convert(int s, int x, int y, const TileMap<int>& tm){
    TexturePtr t = new Texture(s==1?&alive:&dead, win->getRenderer());
    Shape* sha = new Shape(t, {x*PIX_TAILLE, y*PIX_TAILLE, PIX_TAILLE, PIX_TAILLE});
    return sha;
}

void init(){
    for(int x = 0; x<PIX_TAILLE; ++x){
        for(int y = 0; y<PIX_TAILLE; ++y){
            alive.setPixel(x, y, Color::Blue);
            dead.setPixel(x, y, Color::White);
        }
    }
}


void brush(int x, int y, bool justeOui, bool justeNon){
    for(unsigned i = x-tailleBrosse+1; i < x+tailleBrosse; ++i){
        for(unsigned j = y-tailleBrosse+1; j < y+tailleBrosse; ++j){
            if(i>=0 and i <HAUTEUR and j>= 0 and j<LARGEUR){
                if(not justeOui and not justeNon)
                    grid[i][j] = grid[i][j]==1?0:1;
                else if(justeNon){
                    grid[i][j] = not ((not grid[i][j]) or 1);
                }else if(justeOui){
                    grid[i][j] = grid[i][j] or 1;
                }
            }
        }
    }
}

int main()
{
    InputManager::getInstance().addTerminateKey(SDL_SCANCODE_ESCAPE);

    WindowsManager::getInstance().createWindow("Création de formes", PIX_TAILLE*LARGEUR, PIX_TAILLE*HAUTEUR);
    win = Renderer::getInstance().initialize(WindowsManager::getInstance().getWindow("Création de formes"));

    init();

    TileMap<int> tileMap(grid, {0, 0, HAUTEUR, LARGEUR}, &convert);

    //creation

    InputManager::getInstance().addUnfreezed(SDL_SCANCODE_LSHIFT);

    while(InputManager::getInstance().stillRunning()){
        if(InputManager::getInstance().isPressed(SDL_SCANCODE_SPACE))
            break;
        if(InputManager::getInstance().isPressed(SDL_BUTTON_LEFT)){
            int x = InputManager::getInstance().getPointerX();
            int y = InputManager::getInstance().getPointerY();
            x/=PIX_TAILLE;
            y/=PIX_TAILLE;
            brush(x, y, InputManager::getInstance().isPressed(SDL_SCANCODE_LSHIFT),
                  InputManager::getInstance().isPressed(SDL_SCANCODE_LCTRL));
            tileMap.updateAll();
//            InputManager::getInstance().freezeInput(Finput);
        }

        win->clearShape();
        tileMap.rend(win);

        Renderer::getInstance().update();
        InputManager::getInstance().updateEvent();
    }

    InputManager::getInstance().clearEvent();

    std::ofstream stream(fichierOut);
    stream << 2 << "\n" << 1 << "\n" << LARGEUR*HAUTEUR << "\n"; //nombre d'entrées, de sorties, d'exemples

    for(int i = 0; i<HAUTEUR; ++i){
        for(int j = 0; j<LARGEUR; ++j){
            stream << (i-HAUTEUR/2)*pas_h << " " << (j-LARGEUR/2)*pas_l << "\n" << grid[i][j] << "\n";
        }
    }

    stream << endl;

    return 0;
}
