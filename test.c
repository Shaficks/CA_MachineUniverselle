#include <stdio.h>
#include <stdlib.h>

//mask a 4294966847
/* Fonction qui convertit un entier en binaire et stocke le résultat dans le tableau de 32 bits passé en paramètres
void decimalToBinaire(int n, int tab[]) {
  int mask = 1;
  int i, j = 31;
  for (i = 0 ; i < 32 ; ++i) {
    tab[j--] = (n & mask) >> i;
    mask <<= 1;
  }
}
*/
/* Fonction qui convertit un nombre binaire(stocké dans un tableau de 32 bits) en décimal et le retourne */
/* Les paramètres debut et fin serviront à traiter des parties du nombre binaire
int binaireToDecimal(int tab[], int debut, int fin) {
  int i, p = 1, res = 0;
  for(i = debut; i >= fin; i--) {
    res += tab[i] * p;
    p *= 2;
  }
  return res;
}
*/

/* Fonction qui lit un fichier binaire dans le tableau global d'entiers nommé "plateaux" */
void lecture(char* nom_fichier) {
  FILE* f = NULL;
  f = fopen(nom_fichier, "rb");

  //On teste si le pointeur sur le fichier est pas NULL
  if (f != NULL) {
    fseek(f, 0, SEEK_END); //Aller à la fin du fichier
    size = ftell(f); //Lire l'offset de la position courante par rapport au début du fichier
  }
  else {
    printf("\nLecture du fichier échouée ...\n");
    exit(0);
  }

  plateaux = calloc(size/4,sizeof(unsigned int)); //Initialisation de plateaux

  tab_plateaux = calloc(16, sizeof(*tab_plateaux)); //Initialisation de la collection de tableaux de plateaux (programme à l'indice 0)
  int i;
  for(i = 0; i < size/4; i++) {
    tab_plateaux[i] = calloc(size/4, sizeof(*(tab_plateaux[i])));
  }

  fclose(f);

  int fd = open("sandmark.umz", O_RDONLY);

  read(fd,tab_plateaux[0],size/4);
}



/********** OPERATORS **********/

/* 0 - Conditional Move */
void conditionalMove(int i) {
  int c = (7 & tab_plateaux[0][i]);
  //Si le registre C est différent de 0
  if(registre[c]) {
    int b = (tab_plateaux[0][i] & (7 << 3));
    b >>= 3;
    int a = (tab_plateaux[0][i] & (7 << 6));
    a >>= 6;

    registre[a] = registre[b]; //Le registre identifié par a reçoit la valeur de celui identifié par b

  }
}

/* 1 - Array Index */
void arrayIndex(int i) {
  int a,b,c;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6; //Récupération de l'identificateur
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3; //Récupération de l'identificateur
  c = (tab_plateaux[0][i] & 7); //Récupération de l'offset

  registre[a] = tab_plateaux[b][c];
}

/* 2 - Array Amendment */
void arrayAmendment(int i) {
  int a, b;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6;
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3;
  tab_plateaux[a][b] = (tab_plateaux[0][i] & 7);
}

/* 3 - Addition */
void addition(int i) {
  int a,b,c;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6; //Récupération de b
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3; //Récupération de b
  c = (tab_plateaux[0][i] & 7); //Récupération c

  registre[a] = (registre[b]+registre[c])%(int)pow(2,32);
}

/* 4 - Multiplication */
void multiplication(int i) {
  int a,b,c;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6; //Récupération de b
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3; //Récupération de b
  c = (tab_plateaux[0][i] & 7); //Récupération c

  registre[a] = (registre[b]*registre[c])%(int)pow(2,32);

}

/* 5 - Division */
void division(int i) {
  int a,b,c;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6; //Récupération de b
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3; //Récupération de b
  c = (tab_plateaux[0][i] & 7); //Récupération c

if(registre[b] && registre[c])
  registre[a] = registre[b]/registre[c];
}

/* 6 - Not-And */
void notAnd(int i) {
  int a,b,c;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6; //Récupération de b
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3; //Récupération de b
  c = (tab_plateaux[0][i] & 7); //Récupération c

  registre[a] = ~(registre[b] & registre[c]);
}

/* 7 - Halt */
void halt() {
  exit(0);
}

/* 8 - Allocation */
void allocation();

/* 9 - Abandonment */
void abandonment();

/* 10 - Output */
void output();

/* 11 - Input */
void input();

/* 12 - Load Program */
void loadProgram();

/* 13 - Ortography */
void ortography(int i) {
  int a,b,c;
  a = (tab_plateaux[0][i] & (7 << 6));
  a >>= 6; //Récupération de b
  b = (tab_plateaux[0][i] & (7 << 3));
  b >>= 3; //Récupération de b
  c = (tab_plateaux[0][i] & 7); //Récupération c

  registre[a] = (registre[b]+registre[c])%(int)pow(2,32);
}



int main() {
  long int operateur;
  int a = 3489660936;      //0011010000000000000000000000001000


  //Récupération de l'opérateur
  operateur = (long int) (a & (15<<28));
  operateur >>= 28;


  printf("\nA : %u | Operator : %ld",a, operateur);



  printf("\n");

  return EXIT_SUCCESS;
}
