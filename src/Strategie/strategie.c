#include "../Librairies/interface.h"
#include "../Librairies/interfacePerso.h"

SIa IA;//varibale globale permettant de garder accessible les caractéristiques de l'IA courante

/********************************************************************************************
*
* FUNCTION NAME: InitGame
*
* DESCRIPTION: Fonction à appeler au début de chaque partie
*              La stratégie doit compléter la structure SPlayerInfo
*
* ARGUMENT    TYPE             DESCRIPTION
* id          unsigned int     le numero de l'ia
* nbPlayer    unsigned int     le nombre de joueurs
* info        *SPlayerInfo     structure qui contient le nom de la strategie et le nom des joueurs
*
*********************************************************************************************/
void InitGame(unsigned int id, unsigned int nbPlayer, SPlayerInfo *info)
{
  IA.id = id; //sauvegarde de l'id de l'IA
}

/********************************************************************************************
*
* FUNCTION NAME: PlayTurn
*
* DESCRIPTION: Fonction à appeler à chaque tour sur la stratégie et tant que le retour de
*              fonction est vrai et qu'il n'y a pas d'erreur.
*              - Ne pas oublier pour l'arbitre de dupliquer toute la structure map pour chaque appel !
*              - En cas d'erreur, rétablir la carte dans l'état initial avant le premier tour du joueur.
*
* ARGUMENT    TYPE             DESCRIPTION
* map         const *SMap      la carte
* turn        *STurn           le tour courant
*
* RETURNS: 0 coups terminés (ou erreur), 1 structure turn complétée avec un nouveau coup à jouer.
*
*********************************************************************************************/
int PlayTurn(const SMap *map, STurn *turn)
{

  // actuellement:
  // cellFrom = la premiere cellule alliée trouvée
  // cellTo = la premiere cellule ennemi voisine de cellFrom trouvée


  int nbTerritoires = map->nbCells;
  SCell *territoires = map->cells; //tableau de pointeurs de SCell

  int i;
  for(i = 0; i < nbTerritoires; i ++) //parcours des cellules
  {
    if (territoires[i].owner == IA.id ) //si le territoire m'appartient
    {
      turn->cellFrom = territoires[i].id; //définition de la cellule attaquante
      break;
    }
  }
  SCell **voisins = territoires[turn->cellFrom].neighbors; // Tableau de pointeur vers les cellules voisines de turn->cellFrom
  int nbVoisins = territoires[turn->cellFrom].nbNeighbors;
  int j;
  for(j = 0; j < nbVoisins; j++)
  {
    if (voisins[j]->owner != IA.id) //si le territoire voisin est un territoire ennemi
    {
      turn->cellTo = voisins[j]->id; //définition de la cellule attaquée
      break;
    }
  }
  return 1;
}

/********************************************************************************************
*
* FUNCTION NAME: EndGame
*
* DESCRIPTION: fonction à appeler lors de la fin de partie
*
* ARGUMENT    TYPE             DESCRIPTION
* idWinner    unsigned int     l'id du joueur gagnant
*
*********************************************************************************************/
void EndGame(unsigned int idWinner)
{
  //free tous les objets
  //afficher gagnant et score (le nombre de tours)
}


/********************************************************************************************
*
* FUNCTION NAME: CreatePlayer
*
* DESCRIPTION: création d'un nouveau joueur
*
* ARGUMENT    TYPE             DESCRIPTION
* idNewPlayer unsigned int     l'id du joueur
* name        *char            le nom du nouveau joueur
* info        *SPlayerInfo     structure qui contient le nom de la strategie et le nom des joueurs
*
* RETURNS: la structure SPlayerInfo
*
*********************************************************************************************/
SPlayerInfo* CreatePlayer(unsigned int idNewPlayer, char *name, SPlayerInfo *info)
{
  //MAJ du nom du joueur
  snprintf(info -> members[idNewPlayer], sizeof(info -> members[idNewPlayer]), "%s", name); //copie des <arg2> 1ers char de <arg4> dans <arg1>, en rajoutant '\0'
  return info;
}

/********************************************************************************************
*
* FUNCTION NAME: getClusterSize
*
* DESCRIPTION: renvoie la taille de la grappe de cellules
*
* ARGUMENT      TYPE             DESCRIPTION
* map           const *SMap      la carte
* startingCell  *SCell           l'adresse de la cellule de départ
*
* RETURNS: la taille de la grappe de cellules
*
*********************************************************************************************/
int getClusterSize(const SMap *map, SCell *startingCell)
{
  int idPlayer = startingCell->owner; //l'id du joueur propriétaire de la cellule de référence

  int clusterIds[map->nbCells]; //tableau des id des cellules de la grappe, de taille le nombre de cellules de la map, car taille maximale
  int clusterIdsSize = 0; //la taille de la grappe de cellules

  SCell **cellsToTest = malloc((map->nbCells)*sizeof(SCell *)); //tableau de pointeurs de SCell. Les cellules à tester lors du prochain while
  int i;
  for (i = 0; i < (map->nbCells); i++) {
    cellsToTest[i] = malloc(sizeof(SCell));
  }
  cellsToTest[0] = startingCell; //la premiere cellule à tester dans le while est startingCell
  int cellsToTestSize = 1; //la taille du tableau de cellules à tester
  int nextCellToTest = 0; //indice de la prochaine cellule à tester

  while (cellsToTestSize != nextCellToTest) //tant qu'il reste des cellules alliées dont on n'a pas regardé les voisins
  {
    SCell *currentCell = cellsToTest[nextCellToTest]; //adresse de la cellule courante à tester
    clusterIds[clusterIdsSize] = currentCell->id; //on rajoute l'id de la cellule courante aux cellules parcourues
    clusterIdsSize++;
    SCell **voisins = currentCell->neighbors; // Tableau de pointeur vers les cellules voisines de la cellule courante
    int nbVoisins = currentCell->nbNeighbors;
    int j;
    for(j = 0; (j < nbVoisins) ; j++) //parcours des voisins
    {
      if ((voisins[j]->owner == idPlayer) && !isCellInArrayOfCellPointer(voisins[j], cellsToTest, cellsToTestSize)) //si le cellule voisine est un cellule alliée et qu'elle n'a pas déjà dans cellsToTest
      {
        cellsToTest[cellsToTestSize] = voisins[j]; //on l'a rajoute dans la liste des cellules à tester
        cellsToTestSize++;
      }
    }
    nextCellToTest++;
  }

  //libération allocation mémoire
  int k;
  for (k = 0; k < (map->nbCells); k++) {
    free(cellsToTest[k]);
  }
  free(cellsToTest);
  return clusterIdsSize;
}

/********************************************************************************************
*
* FUNCTION NAME: isIntInArray
*
* DESCRIPTION: teste si un entier est dans un tableau d'entiers
*
* ARGUMENT      TYPE             DESCRIPTION
* val           int              la valeur à rechercher
* arr           *int             le tableau d'entiers
* size          int              la taille du tableau
*
* RETURNS: 1 si val est dans arr, 0 sinon
*
*********************************************************************************************/
int isIntInArray(int val, int *arr, int size){
  int i;
  for (i=0; i < size; i++) {
    if (arr[i] == val)
    return 1;
  }
  return 0;
}

/********************************************************************************************
*
* FUNCTION NAME: isCellInArrayOfCellPointer
*
* DESCRIPTION: teste si une adresse de cellule est dans un tableau d'adresses de cellules
*
* ARGUMENT      TYPE             DESCRIPTION
* cell          *SCell           l'adresse de la cellule à rechercher
* arrCell       **SCell          le tableau d'adresses de cellules
* size          int              la taille du tableau
*
* RETURNS: 1 si cell est dans arrCell, 0 sinon
*
*********************************************************************************************/
int isCellInArrayOfCellPointer(SCell *cell, SCell **arrCell, int size){
  int i;
  for (i=0; i < size; i++) {
    if (arrCell[i] == cell) return 1;
  }
  return 0;
}


//LANCEUR
int main(int argc, char const *argv[]) {

  printf("%s\n", " -> dans main de strategie");
  return 0;
}
