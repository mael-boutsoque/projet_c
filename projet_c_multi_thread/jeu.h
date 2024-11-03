#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>
#include <conio.h>

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

#define TRUE 1
#define FALSE 0

// utilisee seulement pour charger le plateau a patir du .txt
enum direction {HAUT,BAS,GAUCHE,DROITE};
typedef struct{
    int **plateau;
    int lignes; // Nombre de lignes de plateau
    int colonnes; // Nobre de colonnes de plateau
    int posl; // ligne actuelle de l'étoile
    int pl; // ligne précédente de l'étoile
    int posc; // colonne actuelle de l'étoile
    int pc; // colonne précédente de l'étoile
    enum direction direction; // direction actuelle de l'étoile
} star_t;


typedef struct{
    int hauteur;
    int largeur;
    int nb_voiture;
    int nb_route_horizontale;
    int nb_route_verticale;
    int d_route_verticale;
    int d_route_horizontale;
} parameters;

typedef struct{
    int **plateau;
    int lignes; // Nombre de lignes de plateau
    int colonnes; // Nobre de colonnes de plateau
    int **voitures; //position des voitures [[y,x],[y,x],...,[y,x]]
    int nb_voitures;
    int **feux; //position des feux de signalisation [[y,x],[y,x],...,[y,x]]
    int nb_feux;
    int d_route_horizontale;
    int d_route_verticale;
    int nb_route_verticale;
    int nb_route_horizontale;
} carte_routiere;

typedef struct{
    int id;
    carte_routiere carte;
} thread_parameters;

void lire_plateau(char *fichier,star_t *star);
void clearScreen();
void *afficher_plateau(void *arg);
