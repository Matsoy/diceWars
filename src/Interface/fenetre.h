#include "generation.h"

typedef void (*pfInitGame)(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info);
typedef int (*pfPlayTurn)(unsigned int idjoueurActuel, const SMap *map, STurn *turn);
typedef void (*pfEndGame)(unsigned int id, unsigned int idWinner);

/********************************************************************************************
*
* FUNCTION NAME: fenetre
*
* DESCRIPTION: Génère la fenêtre de jeu SDL2 et départ du programme (boucle du jeu)
*
*********************************************************************************************/
void fenetre(int nbJoueurs, int nbParties, pfInitGame* tab_InitGame, pfPlayTurn* tab_PlayTurn, pfEndGame* tab_EndGame);

/********************************************************************************************
*
* FUNCTION NAME: displayDices
*
* DESCRIPTION: affichage de l'image des dés de la cellule
*
* ARGUMENT                  TYPE             DESCRIPTION
* renderer                  SDL_Renderer*    Permet d'intérargir avec la fenêtre SDL2
* background_surface_tab    SDL_Surface**    Tableau de pointeurs de SDL_Surface pour les images de dés
* background_texture_tab    SDL_Texture**    Tableau de pointeurs de SDL_Texture pour les images de dés
* pixel_x                   int              coordonnée x du pixel central de la cellule
* pixel_y                   int              coordonnée y du pixel central de la cellule
* idCEll                    int              l'id de la cellule
* nbDices                   int              le nombre de dés de la cellule
*
*********************************************************************************************/
void displayDices(SDL_Renderer* renderer, int pixel_x, int pixel_y, int idCell, int nbDices);
void update_affichage(SMap* map, int x, int y, int h, int w, int** tab_points, int** tab_borduresBlanches, int** tab_id, int* tab_comparaison, int couleurs[8][3], SDL_Renderer* renderer);
