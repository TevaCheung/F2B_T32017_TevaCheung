#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780

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

typedef struct gameState{

    int g_bRunning;
    int left;
    int right;


}gameState;


typedef struct case_plateau{
    SDL_Rect cellule;
    int etat;
}case_plateau;

typedef struct font{
    TTF_Font *g_font;
}font;


enum etat_cellule{
    vide=0,pleine,
};

enum phase_actuelle{
    accueil=0,niveau=1,choix_case=2,editeur=3,editeur_termine=4,reset=5,verif_niveau=6, verif_editeur=7, sauvegarde=8
};

enum BOOL{
    faux=0,vrai,
};

int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame,font *myFont);
void init_boutons(game* myGame, SDL_Rect* clear, SDL_Rect* edit, SDL_Rect* jouer, SDL_Rect* retour,SDL_Rect* save);
void handleEvents(gameState *state, game* myGame);
void cutBitmapTexture(game *myGame,gameState state);
void delay(unsigned int frameLimit);
void destroyTexture(game *myGame);
void destroy(game *myGame);
int isClicked(SDL_Rect sprite, int x, int y);

void affBoutonJouer(game* myGame, SDL_Rect* bouton);
void affBoutonEditer(game* myGame, SDL_Rect* bouton);
void affBoutonRetour(game* myGame, SDL_Rect* bouton);
void affBoutonSave(game* myGame, SDL_Rect* bouton);
void affBoutonClear(game* myGame, SDL_Rect* bouton);

void affCellule(game* myGame, SDL_Rect* cellule, int etat);

int main(int argc, char *argv[])
{
     game myGame;
     gameState state;
     SDL_Rect grille_case[25][25];
     int grille_val[25][25];
     font mFont;
     SDL_Color fontColor={255,255,255};
     int i,j;
     int x,y;
    FILE* fichier = NULL;


    //Pour les 60 fps
    unsigned int frameLimit = SDL_GetTicks() + 16;

    if(init("Clean-up",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame,&mFont)){
            state.g_bRunning=1;
            myGame.phase=accueil;
            myGame.nb_coups=0;
            SDL_RenderClear(myGame.g_pRenderer);
    }else{
            return 1;//something's wrong
    }

    for(i=0;i<25;i++){
        for(j=0;j<25;j++){
            grille_case[i][j].x=i*50;
            grille_case[i][j].y=(j*50)+55;
            grille_val[i][j]=vide;
        }
    }



    myGame.g_texture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);

    while(state.g_bRunning){
            /*
            printf("nb cases : %i\n",myGame.nb_cases);
            printf("phase : %i\n",myGame.phase);
            if (myGame.phase==choix_case){
                printf("phase : choix_case\n");
            }else if(myGame.phase==editeur){
                printf("phase : editeur\n\n");
            }else if(myGame.phase==choix_case){
                printf("phase : choix case\n\n");
            }else if(myGame.phase==niveau){
                printf("phase : niveau\n\n");
            }else if(myGame.phase==editeur_termine){
                printf("phase : editeur termine\n\n");
            }*/

            // ---- ACCUEIL ---- //
            if (myGame.phase==accueil){

                affBoutonJouer(&myGame,&myGame.bouton_jouer);
                affBoutonEditer(&myGame,&myGame.bouton_editer);

                // ---- CHOIX CASE ---- //
            }else if(myGame.phase==choix_case){

                affBoutonPlus(&myGame,&myGame.bouton_plus);
                affBoutonMoins(&myGame,&myGame.bouton_moins);
                affBoutonRetour(&myGame,&myGame.bouton_retour);
                affBoutonSave(&myGame,&myGame.bouton_save);

                // ---- EDITEUR ---- 
            }else if(myGame.phase==editeur){
                for(i=0;i<myGame.nb_cases;i++){
                    for(j=0;j<myGame.nb_cases;j++){
                        affCellule(&myGame,&grille_case[i][j],grille_val[i][j]);
                    }
                }

                affBoutonRetour(&myGame,&myGame.bouton_retour);
                affBoutonClear(&myGame,&myGame.bouton_clear);
                affBoutonSave(&myGame,&myGame.bouton_save);

                myGame.phase=editeur_termine; //pour eviter de recharger la grille à chaque tick

                // ---- EDITEUR TERMINE ---- //
            }else if(myGame.phase==editeur_termine){


                    // ---- SAUVEGARDE ---- //
            }else if(myGame.phase==sauvegarde){
                if(myGame.nb_coups!=0){
                    fichier=fopen("niveau_persos.txt","a+");

                    fprintf(fichier,"<niveau>\n    <nb_cases>%i</nb_cases>\n    <nb_coups>%i</nb_coups>\n",myGame.nb_cases,myGame.nb_coups);

                    for(i=0;i<myGame.nb_cases;i++){
                        for(j=0;j<myGame.nb_cases;j++){
                            if(grille_val[i][j]==pleine){
                                fprintf(fichier,"    <ligne>%i</ligne>\n    <colonne>%i</colonne>\n\n",i,j);
                            }
                        }
                    }

                    fputs("</niveau>\n",fichier);

                    fclose(fichier);
                }

                myGame.phase=reset;

                // ---- VERIF EDITEUR ---- //
            }else if(myGame.phase==verif_editeur){
                myGame.i_case=-1;
                for(i=0;i<myGame.nb_cases;i++){
                    for(j=0;j<myGame.nb_cases;j++){
                        if(isClicked(grille_case[i][j],myGame.x_clic,myGame.y_clic)==vrai){
                            myGame.i_case=i;
                            myGame.j_case=j;
                            break;
                        }
                    }
                }

                if(myGame.i_case>0 && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case-1][myGame.j_case]==vide){
                        grille_val[myGame.i_case-1][myGame.j_case]=pleine;
                    }else{
                        grille_val[myGame.i_case-1][myGame.j_case]=vide;
                    }
                }
                if(myGame.i_case<myGame.nb_cases && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case+1][myGame.j_case]==vide){
                        grille_val[myGame.i_case+1][myGame.j_case]=pleine;
                    }else{
                        grille_val[myGame.i_case+1][myGame.j_case]=vide;
                    }
                }
                if(myGame.j_case>0 && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case][myGame.j_case-1]==vide){
                        grille_val[myGame.i_case][myGame.j_case-1]=pleine;
                    }else{
                        grille_val[myGame.i_case][myGame.j_case-1]=vide;
                    }
                }
                if(myGame.j_case<myGame.nb_cases && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case][myGame.j_case+1]==vide){
                        grille_val[myGame.i_case][myGame.j_case+1]=pleine;
                    }else{
                        grille_val[myGame.i_case][myGame.j_case+1]=vide;
                    }
                }

                if(myGame.i_case!=-1){
                    myGame.nb_coups+=1;
                    printf("Nombre de coups : %i\n",myGame.nb_coups);
                }

                SDL_RenderClear(myGame.g_pRenderer);


                for(i=0;i<myGame.nb_cases;i++){
                    for(j=0;j<myGame.nb_cases;j++){
                        affCellule(&myGame,&grille_case[i][j],grille_val[i][j]);
                    }
                }
                myGame.phase=editeur_termine;
                affBoutonRetour(&myGame,&myGame.bouton_retour);
                affBoutonSave(&myGame,&myGame.bouton_save);
                affBoutonClear(&myGame,&myGame.bouton_clear);

                // ---- RESET ---- //
            }else if(myGame.phase==reset){
                 for(i=0;i<25;i++){
                    for(j=0;j<25;j++){
                        grille_val[i][j]=vide;
                    }
                }
                printf("reset de la grille effectue\n");

                myGame.nb_coups=0;

                myGame.phase=editeur;

                    // ---- NIVEAU ---- //
            }else if(myGame.phase==niveau){
                for(i=0;i<myGame.nb_cases;i++){
                    for(j=0;j<myGame.nb_cases;j++){
                        affCellule(&myGame,&grille_case[i][j],grille_val[i][j]);
                    }
                }

                affBoutonRetour(&myGame,&myGame.bouton_retour);
                // ---- VERFIF NIVEAU ---- //
            }else if(myGame.phase==verif_niveau){
                myGame.i_case=-1;
                for(i=0;i<myGame.nb_cases;i++){
                    for(j=0;j<myGame.nb_cases;j++){
                        if(isClicked(grille_case[i][j],myGame.x_clic,myGame.y_clic)==vrai){
                            myGame.i_case=i;
                            myGame.j_case=j;
                            break;
                        }
                    }
                }

                if(myGame.i_case>0 && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case-1][myGame.j_case]==vide){
                        grille_val[myGame.i_case-1][myGame.j_case]=pleine;
                    }else{
                        grille_val[myGame.i_case-1][myGame.j_case]=vide;
                    }
                }
                if(myGame.i_case<myGame.nb_cases && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case+1][myGame.j_case]==vide){
                        grille_val[myGame.i_case+1][myGame.j_case]=pleine;
                    }else{
                        grille_val[myGame.i_case+1][myGame.j_case]=vide;
                    }
                }
                if(myGame.j_case>0 && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case][myGame.j_case-1]==vide){
                        grille_val[myGame.i_case][myGame.j_case-1]=pleine;
                    }else{
                        grille_val[myGame.i_case][myGame.j_case-1]=vide;
                    }
                }
                if(myGame.j_case<myGame.nb_cases && myGame.i_case!=-1){
                    if(grille_val[myGame.i_case][myGame.j_case+1]==vide){
                        grille_val[myGame.i_case][myGame.j_case+1]=pleine;
                    }else{
                        grille_val[myGame.i_case][myGame.j_case+1]=vide;
                    }
                }

                if(myGame.i_case!=-1){
                    myGame.nb_coups+=1;
                }

                SDL_RenderClear(myGame.g_pRenderer);

                myGame.phase=niveau;
            }


           handleEvents(&state,&myGame);


            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;

            SDL_RenderPresent(myGame.g_pRenderer); // Affichage

    }

        destroy(&myGame);

        //TTF_Quit();
        SDL_Quit();
        free(fichier);
    return 0;
}

/*
BUT: Afficher une cellule vide ou pleine, selon la valeur
ENTREE: L'instance de jeu, la cellule (rect sdl), l'etat de la cellule
SORTIE: Chargement et affichage du sprite de la cellule
*/
void affCellule(game* myGame, SDL_Rect* cellule, int etat){
    SDL_Rect rectangleSource;

    if (etat==vide){
        myGame->g_surface = IMG_Load("./assets/case_vide.png");
    }else if(etat==pleine){
        myGame->g_surface = IMG_Load("./assets/case_remplie.png");
    }

    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface);

    rectangleSource.x=0;
    rectangleSource.y=0;
    rectangleSource.w=50;
    rectangleSource.h=50;

    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

    cellule->w=rectangleSource.w; //Largeur
    cellule->h=rectangleSource.h; //Hauteur

    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,cellule);

    destroyTexture(myGame);
    SDL_FreeSurface(myGame->g_surface);
}

/*
BUT: Initialiser la fenetre, les boutons, la font et l'instance de jeu
ENTREE: La fenetre, les boutons, la font et l'instance de jeu
SORTIE: Toutes les variables setees
*/
int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame, font *myFont){
    //initialize SDL
    int i,j;

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,height,width,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_ACCELERATED);
                SDL_SetRenderDrawColor(myGame->g_pRenderer, 255, 255, 255, 255);
                affBoutonClear(myGame,&myGame->bouton_clear);
                affBoutonRetour(myGame,&myGame->bouton_retour);
                affBoutonSave(myGame,&myGame->bouton_save);
                affBoutonPlus(myGame,&myGame->bouton_plus);
                affBoutonMoins(myGame,&myGame->bouton_clear);
                affBoutonJouer(myGame,&myGame->bouton_jouer);
                affBoutonEditer(myGame,&myGame->bouton_editer);
                SDL_RenderClear(myGame->g_pRenderer);
                myGame->nb_cases=6;
                myGame->i_case=0;
                myGame->j_case=0;
                printf("tout est initie\n");
            }
    }else{
        return 0;
    }

    //myFont->g_font=TTF_OpenFont("./assets/sinanova.ttf",65);

    return 1;
}

/*
BUT: Faire un traitement adapte du clic selon la phase en cours et l'endroit clique
ENTREE: L'etat instance de jeu, l'instance de jeu
SORTIE: Le traitement adapte, clear ecran et passage a une autre etape
*/
void handleEvents(gameState *state, game* myGame){
    SDL_Event event;
    int i,j;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;

        case SDL_MOUSEBUTTONDOWN:
                    switch (myGame->phase)
                    {
                        case accueil: // --- ACCUEIL --- //
                            printf("accueil\n");
                            if (isClicked(myGame->bouton_jouer,event.button.x,event.button.y)==vrai && myGame->phase==accueil){
                                myGame->phase=niveau;
                                SDL_RenderClear(myGame->g_pRenderer);
                            }else if(isClicked(myGame->bouton_editer,event.button.x,event.button.y)==vrai && myGame->phase==accueil){
                                myGame->phase=choix_case;
                                SDL_RenderClear(myGame->g_pRenderer);
                            }
                            break;
                        case choix_case: // --- CHOIX CASE EDITEUR --- //
                            if(isClicked(myGame->bouton_plus,event.button.x,event.button.y)==vrai && myGame->nb_cases<25){
                                    myGame->nb_cases+=1;
                                    printf("Nb cases : %i\n",myGame->nb_cases);
                                    //affiche nb par font ICI
                                    SDL_RenderClear(myGame->g_pRenderer);
                            }else if(isClicked(myGame->bouton_moins,event.button.x,event.button.y)==vrai && myGame->nb_cases>6){
                                    myGame->nb_cases-=1;
                                    printf("Nb cases : %i\n",myGame->nb_cases);
                                    //affiche nb par font ICI
                                    SDL_RenderClear(myGame->g_pRenderer);
                            }else if(isClicked(myGame->bouton_save,event.button.x,event.button.y)==vrai){
                                printf("Nombre de cases sauvegarde\n\n");
                                myGame->phase=editeur;
                                SDL_RenderClear(myGame->g_pRenderer);
                            }else if(isClicked(myGame->bouton_retour,event.button.x,event.button.y)==vrai){
                                myGame->phase=accueil;
                                SDL_RenderClear(myGame->g_pRenderer);
                            }
                            break;
                        case editeur_termine: // --- CHOIX EDITEUR --- //
                            if(isClicked(myGame->bouton_retour,event.button.x,event.button.y)==vrai){
                                myGame->phase=choix_case;
                                SDL_RenderClear(myGame->g_pRenderer);
                            }else if(isClicked(myGame->bouton_save,event.button.x,event.button.y)==vrai){
                                myGame->phase=sauvegarde;
                                printf("Niveau sauvegarde\n");
                                //SDL_RenderClear(myGame->g_pRenderer);
                            }else if(isClicked(myGame->bouton_clear,event.button.x,event.button.y)==vrai){
                                myGame->phase=reset;

                                SDL_RenderClear(myGame->g_pRenderer);
                            }else{
                                myGame->x_clic=event.button.x;
                                myGame->y_clic=event.button.y;
                                myGame->phase=verif_editeur;
                            }
                            break;

                        case niveau:
                            if(isClicked(myGame->bouton_retour,event.button.x,event.button.y)==vrai){
                                myGame->phase=accueil;
                                SDL_RenderClear(myGame->g_pRenderer);
                            }else{
                                myGame->x_clic=event.button.x;
                                myGame->y_clic=event.button.y;
                                myGame->phase=verif_niveau;
                            }
                    }
                    //printf("Phase : %i --- APRES CLIC\n",myGame->phase);


        default:break;

        }
    }
}

/*
BUT: Gerer les fps
ENTREE: Le delai entre deux images
SORTIE: La limite fixee
*/
void delay(unsigned int frameLimit){
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

/*
BUT: Detruire l'instance de jeu
ENTREE: L'instance
SORTIE: La destruction des attributs
*/
void destroy(game *myGame){

    //Destroy render
    if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);


    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

        /*
    if(myFont->g_font!=NULL){
        TTF_CloseFont(myFont->g_font);  //Doit être avant TTF_Quit()
        myFont->g_font=NULL;
    }*/

}

/*
BUT: Detruire la texture d'une instance de jeu
ENTREE: L'instance de jeu
SORTIE: L'instance mise a jour
*/
void destroyTexture(game *myGame){

    //Destroy texture
    if(myGame->g_texture!=NULL)
            SDL_DestroyTexture(myGame->g_texture);


}

/*
BUT: Verifier qu'un sprite est clique ou non
ENTREE: Le sprite et les coord de la souris
SORTIE: vrai si sprit clique, faux sinon
*/
int isClicked(SDL_Rect sprite,int x, int y){
    int clique;
    clique=faux;

    if (x>sprite.x && x<sprite.x+sprite.w){
        if (y>sprite.y && y<sprite.y+sprite.h){
            clique=vrai;
        }
    }

    return clique;
}

/*
BUT: set et afficher le bouton jouer
ENTREE: l'instance de jeu, le bouton
SORTIE: le bouton set et affiche
*/
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


/*
BUT: set et affiche le bouton editer
ENTREE: l'instance de jeu, le bouton
SORTIE: le bouton set et affiche
*/
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

/*
BUT: set et affiche le bouton retour
ENTREE: l'instance de jeu, le bouton
SORTIE: le bouton set et affiche
*/
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

/*
BUT: set et affiche le bouton save
ENTREE: l'instance de jeu, le bouton
SORTIE: le bouton set et affiche
*/
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

/*
BUT: set et affiche le bouton clear
ENTREE: instance de jeu et bouton
SORTIE: le bouton set et affiche
*/
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

/*
/*
BUT: set et affiche le bouton plus
ENTREE: instance de jeu et bouton
SORTIE: le bouton set et affiche
*/
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

/*
/*
BUT: set et affiche le bouton moins
ENTREE: instance de jeu et bouton
SORTIE: le bouton set et affiche
*/
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




