/**
 * \file machine_universlle.c
 * \brief Master 1 Informatique - Spécialité STL - 2015-2016 Semestre 2 - Compilation Avancée
 * \author Chafik NOUIRA
 * \date 06 février 2016
 * \version 1.0
 *
 * Machine Univerelle - Fichier principal du programme :
 *      - Implémentation de la Machine Universelle décrite dans "ICFP Programming Contest 2006"
 *      - Site de l'évènement : http://www.boundvariable.org/index.shtml
 */


/********** Importation des différentes bibliothèques utilisées dans le programme **********/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/********** Déclaration des variables nécessaires pour la machine universelle - En global car ça facilite l'implémentation **********/
unsigned int* plateaux; //Une quantité infinie de plateaux  de sable de 32 marques (bits)
long size; //Taille du fichier chargé
int registre[8] = {0,0,0,0,0,0,0,0}; //8 registres contenant chacun un plateau de sable (Initialisés à zéro)
unsigned int** tab_plateaux; //Collection de tableaux de plateaux
int console; //Console servant à afficher les glyphes
int taille_collection = 1; //La taille de la collection de plateaux est initialisée à 1 (Car le 1er tableau contient le programme)

/********** Program Principal **********/
int main(void) {
  /********** Chargement du Programme à partir du fichier **********/
  int i;
  FILE* f = NULL;
  f = fopen("files/cmu.um", "rb"); //cat um.um sandmark.umz > cmu.um
  //f = fopen("files/cmu2.um", "rb"); //cat um.um codex.um > cmu2.um

  //Récupération de la taille du fichier
  if (f != NULL) {
    fseek(f, 0, SEEK_END); //Aller à la fin du fichier
    size = ftell(f); //Lire l'offset de la position courante par rapport au début du fichier pour récupérer la taille
  }
  else {
    printf("\nLecture du fichier échouée ...\n");
    exit(0);
  }

  /* Allocation de la mémoire nécessaire */
  plateaux = calloc(size/4,sizeof(unsigned int)); //Initialisation des plateaux avec un calloc (initialisation à 0 prise en compte)

  //Initialisation de la collection de tableaux de plateaux (programme chargé dans le tableau d'identificateur 0)
  tab_plateaux = calloc(taille_collection, sizeof(*tab_plateaux));
  tab_plateaux[0] = calloc(size/4, sizeof(*(tab_plateaux[0])));

  fseek(f,0,SEEK_SET); //Remettre le pointeur au début du fichier afin d'en charger le contenu par la suite

  i = 0;
  //int fd = open("files/um.um", O_RDONLY); //Ré-Ouverture du fichier en utilisant open() à la place de fopen()
  //read(fd,tab_plateaux[0],size/4); //Chargement du programme dans la tableau d'identificateur 0 de la collection de tableaux de plateaux
	while(!feof(f)){
 		tab_plateaux[0][i] |= (unsigned int)(fgetc(f) << 24);
 		tab_plateaux[0][i] |= (unsigned int)(fgetc(f) << 16);
 		tab_plateaux[0][i] |= (unsigned int)(fgetc(f) << 8);
 		tab_plateaux[0][i] |= (unsigned int) fgetc(f) ;
		i++;
	}

  fclose(f); //Fermeture du fichier

  // Affichage pour débug !!!!!!!!!!!!!!!!!!!!!
  for(i = 0; i < 20; i++)
    printf("\nCollection Plateaux %d : %x", i, tab_plateaux[0][i]);
  printf("\n");

int j = 0;
  /********** Boucle principale d'exécution du Programme **********/
  i = -1; //à partir de ce point, i représente l'index d'exécution (incrémentée de 1 en début de boucle)
  while(1) {
    j++;
    i++; if(i == 50) break;
    unsigned int A, B, C; //Variables qui contiendront les index des registres A, B et C.
    long int operateur; //Variable qui contiendra l'opérateur

    //Récupération de l'index du registre A en appliquant un mask sur les 3 bits concernés
    A = (tab_plateaux[0][i] & (7 << 6));
    A >>= 6;
    //Récupération de l'index du registre B
    B = (tab_plateaux[0][i] & (7 << 3));
    B >>= 3;
    //Récupération de l'index du registre C
    C = (tab_plateaux[0][i] & 7);

    //Récupération de l'opérateur
    operateur = (long int) (tab_plateaux[0][i]) & (15<<28);
    operateur >>= 28;


    printf("Boucle %d | Operator : %ld | A : %u B : %u C : %u\n",i,operateur,registre[A],registre[B],registre[C]);
    int valeur; //
    switch(operateur) {
    case 0 : /** Conditional Move **/
        //Si C != 0, le registre identifé par A reçoit la valeur du registre identifié par B
        if(registre[C] != 0) {
            registre[A] = registre[B];
        }
      break;

    case 1 : /** Array Index **/
      //Le registre A reçoit la valeur à l'offset contenu dans C dans le tableau identifé par B
      registre[A] = tab_plateaux[registre[B]][registre[C]];
      break;

    case 2 : /** Array Amendment **/
      //Le tableau identifié par le registe A est modifié à l'offset B par la valeur contenue par C
      tab_plateaux[registre[A]][registre[B]] = registre[C];
      break;

    case 3 : /** Addition **/
      //Le registre A reçoit la valeur contenue dans le registre B plus la valeur contenue par le registre C le tout modulo 2³²
      registre[A] = (registre[B]+registre[C]); //%(int)pow(2,32);
      break;

    case 4 : /** Muliplication **/
      //Le registre A reçoit la valeur contenue dans le registre B multipliée par la valeur contenue par le registre C le tout modulo 2³²
      registre[A] = (registre[B]*registre[C]); //%(int)pow(2,32);
      break;

    case 5 : /** Division **/
      //Le registre A reçoit la valeur contenue dans le registre B divisée par la valeur contenue par le registre C
      if(registre[C]) //Si registre[C] est différent de 0 (Afin d'éviter une division par 0)
        registre[A] = registre[B]/registre[C];
      break;

    case 6 : /** Not-And **/
      //Le registre A reçoit le NAND des registres B et C
      registre[A] = ~(registre[B] & registre[C]);
      break;

    case 7 : /** Halt **/
      //Stoppe la Machine Universelle
      exit(0);
      break;

    case 8 : /** Allocation **/
      //Allocation d'un nouveau tableau de capacité C. Ses cases sont initialisées à 0. B reçoit l'identificateur du nouveau tableau
      ++taille_collection;
      tab_plateaux = realloc(tab_plateaux, taille_collection * sizeof(*tab_plateaux));
      tab_plateaux[taille_collection-1] = calloc(registre[C], sizeof(*(tab_plateaux[taille_collection-1])));
      registre[B] = taille_collection-1;
      break;

    case 9 : /** Abandonment **/
      //Le tableau identifié par C est abandonné et une future allocation peut réutiliser son identificateur
      //free(tab_plateaux[registre[C]]);
      break;

    case 10: /** Output **/
      //Ecrit sur la console la valeur du registre C, seules des valeurs comprises entre 0 et 255 sont tolérées
      if(registre[C] >= 0 && registre[C] <= 255) {
        printf("\nValeur du registre C : %u\n",registre[C]);
        console = registre[C];
      }
      break;

    case 11: /** Input **/
      //La MU attend une entrée sur la console. Le registre C est chargé avec cette entrée. Si la fin de l'entrée est signalée, les bits de C passent tous à 1
      printf("\nVeuillez entrer un nombre, ce dernier sera stocké dans le registre C : ");
      scanf("%d",&registre[C]);
      break;

    case 12: /** Load Program **/
      //Une copie du tableau dont l'identificateur est B remplace le programme principal (ID 0)
      //L'index d'exécution est modifié par la valeur contenue par C
      free(tab_plateaux[0]);
      if(registre[B] != 0) {
        memcpy(tab_plateaux[0],tab_plateaux[registre[B]],sizeof(*tab_plateaux[registre[B]]));
        //tab_plateaux[0] = (unsigned int*)tab_plateaux[registre[B]];
      }
      //if(registre[C] < 256)
      i = registre[C]- 1; //L'index d'exécution reçoit la valeur du registre C -1 car il est réincrémenté au début de la boucle
      break;

    case 13 : /** Orthography **/
      //La valeur indiquée doit être chargée dans le registre A (A : les 3 bits directement après l'opérateur, la valeur est les 25 bits restants)
      valeur = (int) (tab_plateaux[0][i] & 33554431);
      A = (int) (tab_plateaux[0][i] & (7<<25));
      A >>= 25;
      registre[A] = valeur;
      B = C = 0;
      break;

    default: return EXIT_FAILURE; break;

    }
  }


  for(i = 0; i < 8; i++)
    printf("\nRegistre %d : %u", i, registre[i]);


  printf("\nFIN");
  printf("\n\n");
  return EXIT_SUCCESS;
}
