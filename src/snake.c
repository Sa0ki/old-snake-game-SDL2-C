#include "snake.h"

void erreur(char message[])
{
    SDL_Log("ERREUR : %s -> %s\n", message, SDL_GetError());
    exit(EXIT_FAILURE);
}

SDL_Surface *surface(Image *i, char source[], char message[])
{
    if(!i->surface)
    {
        if(!(i->surface = IMG_Load(source)))
        {
            SDL_DestroyRenderer(jeu.rendu);
            SDL_DestroyWindow(jeu.fenetre);
            erreur(message);      
        }
    }

    return i->surface;
}

SDL_Texture *texture(SDL_Rect *rec, Image *i, char source[], char message[], int x, int y)
{
        if(!(i->texture = SDL_CreateTextureFromSurface(jeu.rendu, i->surface)))
        {
            SDL_FreeSurface(i->surface);
            SDL_DestroyRenderer(jeu.rendu);
            SDL_DestroyWindow(jeu.fenetre);
            erreur(message);
        } 

        rec->x = x;
        rec->y = y; 

        if((SDL_QueryTexture(i->texture, NULL, NULL, &rec->w, &rec->h)))
        {
            SDL_FreeSurface(i->surface);
            SDL_DestroyRenderer(jeu.rendu);
            SDL_DestroyWindow(jeu.fenetre);
            erreur("Query textutre ");
        }

    return i->texture;
}

void image(SDL_Rect *rec, Image *i, char source[], char nom[], int x, int y)
{
    if(!i->texture)
    {
        i->surface = surface(i, source, nom);   
        i->texture = texture(rec, i, source, nom, x, y);         
        SDL_FreeSurface(i->surface);
    }

    rec->x = x;
    rec->y = y; 
        
    if(SDL_RenderCopy(jeu.rendu, i->texture, NULL, rec))
    {
        SDL_FreeSurface(i->surface);
        SDL_DestroyTexture(i->texture);
        SDL_DestroyRenderer(jeu.rendu);
        SDL_DestroyWindow(jeu.fenetre);
        erreur("RenderCopy");
    }
}

void dropFruit(Fruit *x)
{   
    do 
    {
        x->lig = rand() % largeur;
        x->col = rand() % longueur;
    }while(x->lig <= png / 2 || x->col <= png / 2 || x->lig >= largeur - 5 * (png / 2) || x->col >= longueur - 5 * (png / 2));

    while((x->lig) % 5)
        x->lig ++;

    while((x->col) % 5)
        x->col ++;     

    //printf("fruit :  x  -> %d  y -> %d\n",x->lig,x->col); 

    if(x->lig >= largeur - 5 * eight)
    {
        printf("fruit :  x  -> %d  y -> %d",x->lig,x->col);   
        x->lig  -= 45;
        printf("\tcorrection x : %d\n",x->lig);    
    }
       
    if(x->col >= longueur - 5 * eight)
    {
        printf("fruit :  x  -> %d  y -> %d",x->lig,x->col);  
        x->col -= 55;
        printf("\tcorrection y : %d\n",x->col);    
    }
}

void menu()
{
    jeu.play = SDL_FALSE;

    while(!jeu.play)
    {   
        SDL_Event evenement;   

        while(SDL_PollEvent(&evenement))
        {     
            switch(evenement.type)
            {
                case SDL_QUIT:
                    jeu.play = SDL_TRUE;
                    jeu.etat = SDL_FALSE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                if(evenement.button.x >= 650 && evenement.button.x <= 650 + 300 && evenement.button.y >= 550 && evenement.button.y <= 550 + 200)
                jeu.play = SDL_TRUE;
                break;

                default:
                break;
            }
        }
        
        image(&jeu.menu.rec, &jeu.menu, "src/img/menu.png", "menu", 0, 0);
        image(&jeu.start.rec, &jeu.start, "src/img/start.png", "start", 650, 550);
        SDL_RenderPresent(jeu.rendu);
    }

    for(int i = 255; i >= 0; i -=4)
    {
        SDL_SetTextureAlphaMod(jeu.menu.texture,i);
        SDL_SetTextureAlphaMod(jeu.start.texture,i);
        image(&jeu.menu.rec, &jeu.menu, "src/img/menu.png", "menu", 0, 0);
        image(&jeu.start.rec, &jeu.start, "src/img/start.png", "start", 650, 550);
        SDL_RenderPresent(jeu.rendu);        
        SDL_Delay(10);
    }
}

void init()
{
    jeu.fenetre = NULL;
    jeu.rendu = NULL;
    snake.tete_up.surface = NULL;
    snake.tete_up.texture = NULL;
    snake.tete_down.surface = NULL;
    snake.tete_down.texture = NULL;
    snake.tete_left.surface = NULL;
    snake.tete_left.texture = NULL;
    snake.tete_right.surface = NULL;
    snake.tete_right.texture = NULL;
    snake.tail_up.surface = NULL;
    snake.tail_up.texture = NULL;
    snake.tail_down.surface = NULL;
    snake.tail_down.texture = NULL;
    snake.tail_left.surface = NULL;
    snake.tail_left.texture = NULL;
    snake.tail_right.surface = NULL;
    snake.tail_right.texture = NULL;
    jeu.background.surface = NULL;
    jeu.background.texture = NULL;
    jeu.menu.surface = NULL;
    jeu.menu.texture = NULL;
    jeu.police = NULL;
    jeu.score.surface = NULL;
    jeu.score.texture = NULL;
    jeu.over.surface = NULL;
    jeu.over.texture = NULL;
    jeu.win.surface = NULL;
    jeu.win.texture = NULL;
    jeu.restart.surface = NULL;
    jeu.restart.texture = NULL;
    jeu.etat = SDL_TRUE;
    jeu.tick_arrive = 0;
    jeu.tick_arrive = 0;
    jeu.dt = 1;
    snake.taille = 0;
    snake.path = -1;
    joueur.score = 0;
    debut = NULL;
    fin  = NULL;
    liste_taille = 0;
    vitesse = 0;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        erreur("Initialisation");

    if(!(jeu.fenetre = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    longueur, largeur, 0)))
        erreur("Creation fenetre"); 

    if(!(jeu.rendu = SDL_CreateRenderer(jeu.fenetre, -1, SDL_RENDERER_ACCELERATED)))
    {
        SDL_DestroyWindow(jeu.fenetre);       
        erreur("Creation Rendu");
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    jeu.point = Mix_LoadWAV("src/msc/point.wav");
    jeu.w = Mix_LoadWAV("src/msc/win.wav");
    jeu.l = Mix_LoadWAV("src/msc/lose.wav");

    menu();

    TTF_Init();
    jeu.police = TTF_OpenFont("bin/police.ttf", 30);
    SDL_Color blanc  = {255, 255, 255};
    jeu.score.rec.x = 20 ;
    jeu.score.rec.y =  20;

    snake.col = (longueur - png)  / 2; //colones
    snake.lig = (largeur - png)  / 2;  //lignes

    dropFruit(&pomme);
    image(&jeu.background.rec, &jeu.background, "src/img/background.png", "background", 0, 0);
    image(&pomme.image.rec, &pomme.image, "src/img/pomme.png", "tete_up", pomme.col, pomme.lig);
    image(&snake.tete_up.rec, &snake.tete_up, "src/img/snake_up.png", "tete_up", snake.col, snake.lig);
}

void replay()
{
    jeu.etat = SDL_TRUE;
    jeu.tick_arrive = 0;
    jeu.tick_arrive = 0;
    jeu.dt = 1;    
    snake.path = -1;
    snake.taille = 0;
    joueur.score = 0;
    debut = NULL;
    fin  = NULL;
    liste_taille = 0;
    vitesse = 0;

    jeu.police = TTF_OpenFont("bin/police.ttf", 30);
    SDL_Color blanc  = {255, 255, 255};
    jeu.score.rec.x = 20 ;
    jeu.score.rec.y =  20;

    snake.col = (longueur - png)  / 2; //colones
    snake.lig = (largeur - png)  / 2;  //lignes

    dropFruit(&pomme);
    image(&jeu.background.rec, &jeu.background, "src/img/background.png", "background", 0, 0);
    image(&pomme.image.rec, &pomme.image, "src/img/pomme.png", "tete_up", pomme.col, pomme.lig);
    image(&snake.tete_up.rec, &snake.tete_up, "src/img/snake_up.png", "tete_up", snake.col, snake.lig);
}

int vider_liste()
{
    element *temp = debut;
    
    while(debut)
    {
        debut = debut->suivant;
        free(temp);
    }

    return 1;
}

void move(direction p)
{
    if(vitesse > 0)
    jeu.dt = 2;
    else
    jeu.dt = 1;

    switch(p)
    {
        case haut:
            snake.lig -= scale * jeu.dt;
            if(jeu.dt == 2)
            image(&snake.tete_up_y.rec, &snake.tete_up_y, "src/img/snake_up_y.png", "tete_up_y", snake.col, snake.lig);
            else
            image(&snake.tete_up.rec, &snake.tete_up, "src/img/snake_up.png", "tete_up", snake.col, snake.lig);
            break;
        
        case bas:
            snake.lig += scale * jeu.dt;
            if(jeu.dt == 2)
            image(&snake.tete_down_y.rec, &snake.tete_down_y, "src/img/snake_down_y.png", "tete_down_y", snake.col, snake.lig);
            else
            image(&snake.tete_down.rec, &snake.tete_down, "src/img/snake_down.png", "tete_down", snake.col, snake.lig);
            break;

        case droite:
            snake.col += scale * jeu.dt;
            if(jeu.dt == 2)
            image(&snake.tete_right_y.rec, &snake.tete_right_y, "src/img/snake_right_y.png", "tete_right_y", snake.col, snake.lig);
            else
            image(&snake.tete_right.rec, &snake.tete_right, "src/img/snake_right.png", "tete_right", snake.col, snake.lig);
            break;

        case gauche:
            snake.col -= scale * jeu.dt;
            if(jeu.dt == 2)
            image(&snake.tete_left_y.rec, &snake.tete_left_y, "src/img/snake_left_y.png", "tete_left_y", snake.col, snake.lig);
            else
            image(&snake.tete_left.rec, &snake.tete_left, "src/img/snake_left.png", "tete_left", snake.col, snake.lig);
            break;
        
        default:
            image(&snake.tete_up.rec, &snake.tete_up, "src/img/snake_up.png", "tete_up", snake.col, snake.lig);
            break;
    }

    if(snake.lig <= 0 || snake.col <= 0 || snake.lig >= largeur - 5*eight|| snake.col >= longueur - 5*eight)
    {
        jeu.final_score = joueur.score;
        joueur.score = 0;
        if(vider_liste()) //printf("Liste vide\n");
        jeu.play = SDL_FALSE;
    }

    if(debut && snake.taille > 9)
    {
        element *temp = debut;

        for(int i = 0; i < 9 ; i ++)
        temp = temp->suivant;

        while(temp)
        {
            for(int i = snake.lig; i <= snake.lig + scale; i += scale)
            {
                for(int j = snake.col; j <= snake.col + scale; j += scale)
                {
                    if(i >= temp->x && i <= temp->x + png / 2 && j >= temp->y && j <= temp->y + png / 2)
                    {
                        jeu.final_score = joueur.score;
                        joueur.score = 0;
                        vider_liste();
                        jeu.play = SDL_FALSE;
                        goto fin;
                    }
                }
            }

            temp = temp->suivant;
        }  
    }

    if(jeu.dt == 2)
    vitesse --;

    for(int i = snake.lig; i <= snake.lig + png; i += 5)
    {
        for(int j = snake.col; j <= snake.col + png; j += 5)
        {
            if(i >= pomme.lig && i <= pomme.lig+ png - scale && j >= pomme.col && j <= pomme.col + png - scale)
            {
                Mix_PlayChannel(-1, jeu.point, 0);
                dropFruit(&pomme);
                joueur.score += 10;
                snake.taille ++;
                pomme.eaten = 1;
                vitesse = 6;
                goto fin; 
            }
        }
    }

    fin: return;
}

void save(int x, int y)
{
    element *contenu = malloc(sizeof(element));
    if(!contenu) exit(EXIT_FAILURE);

    contenu->x = x;
    contenu->y = y;
    contenu->suivant = NULL;
    contenu->precedent = NULL;

    if(!debut)
    {
        debut = contenu;
        fin = contenu;
    }
    else
    {
        contenu->precedent = fin;
        fin->suivant = contenu;
        fin = contenu;  
    }

    liste_taille ++;
}

void tails_position()
{
    if(liste_taille != max)
    {
        if(snake.taille > 0 && pomme.eaten)
        {
            save(snake.lig, snake.col); 
            pomme.eaten = 0;
        }

        if(debut)
        {
            element *temp = fin;

            while(temp->precedent)
            {
                temp->x = temp->precedent->x;
                temp->y = temp->precedent->y;
                temp = temp->precedent;
            }

            temp->x = snake.lig;
            temp->y = snake.col;
        }
    }
    else
    {
        jeu.play = SDL_FALSE;
        vider_liste();
    }
}

void tails_display()
{
    if(debut)
    {
        element *temp = debut;

        while(temp)
        {
            switch(snake.path)
            {
                case haut:
                if(jeu.dt == 2)
                image(&snake.tail_up.rec, &snake.tail_up_y, "src/img/snake_tail_up_y.png", "tail_up_y", temp->y, temp->x);
                else
                image(&snake.tail_up.rec, &snake.tail_up, "src/img/snake_tail_up.png", "tail_up", temp->y, temp->x);
                break;
                        
                case bas:
                if(jeu.dt == 2)
                image(&snake.tail_down_y.rec, &snake.tail_down_y, "src/img/snake_tail_down_y.png", "tail_down_y", temp->y, temp->x);
                else
                image(&snake.tail_down.rec, &snake.tail_down, "src/img/snake_tail_down.png", "tail_down", temp->y, temp->x);
                break;

                case droite:
                if(jeu.dt == 2)
                image(&snake.tail_right_y.rec, &snake.tail_right_y, "src/img/snake_tail_right_y.png", "tail_right_y", temp->y, temp->x);
                else
                image(&snake.tail_right.rec, &snake.tail_right, "src/img/snake_tail_right.png", "tail_right", temp->y, temp->x);
                break;

                case gauche:
                if(jeu.dt == 2)
                image(&snake.tail_left_y.rec, &snake.tail_left_y, "src/img/snake_tail_left_y.png", "tail_left_y", temp->y, temp->x);
                else
                image(&snake.tail_left.rec, &snake.tail_left, "src/img/snake_tail_left.png", "tail_left", temp->y, temp->x);
                break;

                default: break;
            }

            temp = temp->suivant;                
        }
    }
}

void choix(SDL_Event e)
{
    switch(e.key.keysym.sym)
    {
        case SDLK_UP:
            snake.path = haut;                
            break;
                        
        case SDLK_DOWN:
            snake.path = bas;               
            break;

        case SDLK_RIGHT:
            snake.path = droite;                   
            break;

        case SDLK_LEFT:
            snake.path = gauche;                  
            break;

        default:
            break;
    }
}

void score(int x)
{
    char module[8] = {'S', 'C', 'O', 'R', 'E', ' ', ':', ' '}, texte[13];

    strcpy(texte, module);
    SDL_Color blanc  = {255, 255, 255};

    texte[7] = ' ';

    if(joueur.score != max_points)
    {  
        texte[8] = '0';
        texte[9] = (x / 100) + '0';
        texte[10] = ((x % 100) / 10) + '0';
        texte[11] = ((x % 100) % 10) + '0';
    }
    else
    {
        texte[8] = '1';
        texte[9] = '0';
        texte[10] = '0';
        texte[11] = '0';
    }


    if(jeu.score.texture)
    {
        SDL_FreeSurface(jeu.score.surface);
        SDL_DestroyTexture(jeu.score.texture);
    }
    
    
    jeu.score.surface = TTF_RenderText_Blended(jeu.police, texte, blanc);
    jeu.score.texture = texture(&jeu.score.rec, &jeu.score, "bin/police.ttf", "texte score", jeu.score.rec.x, jeu.score.rec.y);

    if(SDL_RenderCopy(jeu.rendu, jeu.score.texture, NULL, &jeu.score.rec))
    {
        SDL_FreeSurface(jeu.score.surface);
        SDL_DestroyTexture(jeu.score.texture);
        SDL_DestroyRenderer(jeu.rendu);
        SDL_DestroyWindow(jeu.fenetre);
        erreur("RenderCopy Texte Score");
    }
}

void deplacement()
{
   image(&jeu.background.rec, &jeu.background, "src/img/background.png", "background", 0, 0); // Affiche le background
   image(&pomme.image.rec, &pomme.image, "src/img/pomme.png", "pomme", pomme.col, pomme.lig);  // Affiche le fruit 

   tails_position(); // Sauvegarde les emplacements de la tete en cas d'obtention de point
   tails_display(); // Affiche les différentes parties du snake quand il mange un fruit  

   move(snake.path); // Maj des coordonées & des vérifications et affichage du snake
   score(joueur.score);
   SDL_RenderPresent(jeu.rendu); 
}

void fps()
{    
    if(SDL_GetTicks() < jeu.tick + 1000)
        jeu.fps ++;
    else
    {
        jeu.tick = SDL_GetTicks();
        jeu.fps = 0;
    }

    jeu.tick_arrive = SDL_GetTicks();

    if(jeu.tick_arrive - jeu.tick_depart > 1000/frame)
        jeu.difference_tick = 1000/frame;
    else
        jeu.difference_tick = jeu.tick_arrive - jeu.tick_depart;

    SDL_Delay(1000/frame - jeu.difference_tick);
}

void end()
{
    if(!jeu.play || joueur.score == max_points)
    {
        if(!jeu.play)
            Mix_PlayChannel(-1, jeu.l, 0);
        else if(joueur.score == max_points)
            Mix_PlayChannel(-1, jeu.w, 0);

        jeu.police = TTF_OpenFont("bin/police.ttf", 40);
        jeu.score.rec.x = 345;
        jeu.score.rec.y = 50;

        int verification = 0;

        while(!verification)
        {   
            SDL_Event evenement;   

            while(SDL_PollEvent(&evenement))
            {     
                switch(evenement.type)
                {
                    case SDL_QUIT:
                    jeu.play = SDL_FALSE;
                    jeu.etat = SDL_FALSE;
                    verification = 1;
                    break;

                    case SDL_MOUSEBUTTONDOWN:
                    if(evenement.button.x >= 750 && evenement.button.x <= 750 + 200 && evenement.button.y >= 650 && evenement.button.y <= 650 + 100)
                    {
                      jeu.etat = SDL_TRUE;  
                      jeu.play = SDL_TRUE;
                      verification = 1;
                    }
                    break;

                    default:
                    break;
                }
            }
            
            if(joueur.score == max_points)
            {
                image(&jeu.win.rec, &jeu.win, "src/img/win.png", "Game Over", 0, 0);
                image(&jeu.restart.rec, &jeu.restart, "src/img/restart.png", "Restart", 750, 650);
                score(joueur.score);
            }

            else if(!jeu.play)
            {
                image(&jeu.over.rec, &jeu.over, "src/img/over.png", "Game Over", 0, 0);
                image(&jeu.restart.rec, &jeu.restart, "src/img/restart.png", "Restart", 750, 650);
                score(jeu.final_score);
            }

        SDL_RenderPresent(jeu.rendu);
            
        }

        if(jeu.etat && jeu.play)
            replay();
    }
}

void destroy()
{
    TTF_CloseFont(jeu.police);
    TTF_Quit();
    Mix_FreeChunk(jeu.w);
    Mix_FreeChunk(jeu.l);
    Mix_FreeChunk(jeu.point);
    Mix_CloseAudio();
    SDL_FreeSurface(jeu.over.surface);
    SDL_FreeSurface(jeu.win.surface);
    SDL_FreeSurface(jeu.restart.surface);
    SDL_DestroyTexture(jeu.over.texture);
    SDL_DestroyTexture(jeu.win.texture);
    SDL_DestroyTexture(jeu.score.texture);
    SDL_DestroyTexture(jeu.start.texture);
    SDL_DestroyTexture(jeu.restart.texture);
    SDL_DestroyTexture(snake.tete_right.texture);
    SDL_DestroyTexture(snake.tete_left.texture);
    SDL_DestroyTexture(snake.tete_down.texture);
    SDL_DestroyTexture(snake.tete_up.texture);
    SDL_DestroyTexture(snake.tail_up.texture);
    SDL_DestroyTexture(snake.tail_down.texture);
    SDL_DestroyTexture(snake.tail_left.texture);
    SDL_DestroyTexture(snake.tail_right.texture);
    SDL_DestroyTexture(snake.tete_right_y.texture);
    SDL_DestroyTexture(snake.tete_left_y.texture);
    SDL_DestroyTexture(snake.tete_down_y.texture);
    SDL_DestroyTexture(snake.tete_up_y.texture);
    SDL_DestroyTexture(snake.tail_up_y.texture);
    SDL_DestroyTexture(snake.tail_down_y.texture);
    SDL_DestroyTexture(snake.tail_left_y.texture);
    SDL_DestroyTexture(snake.tail_right_y.texture);
    SDL_DestroyRenderer(jeu.rendu);
    SDL_DestroyWindow(jeu.fenetre);
    SDL_Quit();
}

