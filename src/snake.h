#ifndef __SNAKE__
#define __SNAKE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL.h>

#define longueur 1000 //GetSystemMetrics(SM_CXSCREEN)
#define largeur 800 //GetSystemMetrics(SM_CYSCREEN)
#define max_points 1000
#define max 300
#define scale 10
#define png 40
#define eight 8
#define frame 60

typedef struct 
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rec;
} Image, Texte;

typedef struct 
{
    SDL_Window *fenetre; 
    SDL_Renderer *rendu; 
    TTF_Font *police;
    SDL_bool etat, play;
    Mix_Chunk *point, *w, *l;
    Image background, menu, start, over, win, restart;
    Texte score;
    int tick, tick_depart, tick_arrive, difference_tick, dt, fps, final_score;
} Jeu;

typedef enum
{
    haut = 1,
    bas,
    gauche,
    droite
}direction;

typedef struct 
{
    int col, lig, taille, X[max], Y[max];
    direction path;
    Image tete_up, tete_down, tete_left, tete_right,
          tail_up, tail_down, tail_left, tail_right,
          tete_down_y, tete_up_y, tete_right_y, tete_left_y,
          tail_down_y, tail_up_y, tail_right_y, tail_left_y;
} Snake;

typedef struct
{
    int col, lig, eaten;
    Image image;
} Fruit;

typedef struct
{
    int score;
} Joueur;

typedef struct element
{
    int x, y;
    struct element *suivant;
    struct element *precedent;
}element;

Jeu jeu;
Snake snake;
Fruit pomme;
Joueur joueur;

element *debut;
element *fin;
int liste_taille;
int vitesse;

void erreur(char []);
SDL_Surface *surface(Image *, char [], char []);
SDL_Texture *texture(SDL_Rect *, Image *, char [], char [], int, int);
void image(SDL_Rect *, Image *, char [], char [], int , int);
void dropFruit(Fruit *);
void menu();
void end();
void end_win();
void init();
void move(direction);
void save(int, int);
void tails_position();
void tails_display();
int vider_liste();
void choix(SDL_Event);
void score(int);
void replay();
void fps();
void deplacement();
void destroy();

#endif