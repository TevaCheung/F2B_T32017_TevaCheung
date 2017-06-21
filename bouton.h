#ifndef BOUTON_H_INCLUDED
#define BOUTON_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct game{
     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_texture;
     SDL_Surface *g_surface;
    SDL_Rect bouton_jouer;
    SDL_Rect bouton_editer;
    SDL_Rect bouton_retour;
    SDL_Rect bouton_clear;
    SDL_Rect bouton_save;
    SDL_Rect bouton_plus;
    SDL_Rect bouton_moins;
    int phase;
    int nb_cases;
    int x_clic;
    int y_clic;
    int i_case;
    int j_case;
    int nb_coups;
}game;

void affBoutonJouer(game* myGame, SDL_Rect* bouton);
void affBoutonEditer(game* myGame, SDL_Rect* bouton);
void affBoutonRetour(game* myGame, SDL_Rect* bouton);
void affBoutonSave(game* myGame, SDL_Rect* bouton);
void affBoutonClear(game* myGame, SDL_Rect* bouton);


#endif // BOUTON_H_INCLUDED
