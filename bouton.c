#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "bouton.h"

void affBoutonJouer(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_jouer.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=100;
    rectangleSource.h=50;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=SCREEN_WIDTH/2;//debut x
    bouton->y=200;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);

    if(!myGame->g_surface){
        printf("Pbm chargement");
    }
}

void affBoutonEditer(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_editer.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);


    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=100;
    rectangleSource.h=50;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=SCREEN_WIDTH/2;//debut x
    bouton->y=400;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}

void affBoutonRetour(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_retour.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=50;
    rectangleSource.h=40;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=0;//debut x
    bouton->y=0;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}

void affBoutonSave(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_save.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=50;
    rectangleSource.h=40;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=50;//debut x
    bouton->y=0;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}

void affBoutonClear(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_clear.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=50;
    rectangleSource.h=40;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=100;//debut x
    bouton->y=0;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}

void affBoutonPlus(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_plus.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=50;
    rectangleSource.h=50;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=(SCREEN_WIDTH/2)-100;//debut x
    bouton->y=200;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}

void affBoutonMoins(game* myGame, SDL_Rect* bouton){
    SDL_Rect rectangleSource;

    myGame->g_surface = IMG_Load("./assets/bouton_moins.png");

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=50;
    rectangleSource.h=50;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    bouton->x=(SCREEN_WIDTH/2)+100;//debut x
    bouton->y=200;//debut y
    bouton->w=rectangleSource.w; //Largeur
    bouton->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,bouton);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}
