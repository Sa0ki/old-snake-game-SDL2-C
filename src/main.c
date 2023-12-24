#include "snake.h"

int main(int argc, char **argv)
{
    init();

    while(jeu.etat)
    {    
        jeu.tick_depart = SDL_GetTicks();
        SDL_Event evenement;   

        while(SDL_PollEvent(&evenement))
        {     
            switch(evenement.type)
            {
                case SDL_QUIT:
                    jeu.etat = SDL_FALSE;
                    break;

                case SDL_KEYDOWN:
                    choix(evenement);
                    break;

                default:
                    break;
            }
        }

        deplacement();
        fps();
        end();
    }
        
    destroy();
    
    return 0;
}