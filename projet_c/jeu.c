#include "utilitaires.c"
pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    star_t star;
    star.direction = GAUCHE;
    pthread_t anim,clavier;
    srand(time(0));
	parameters parametres_carte = lire_plateau2("plateau2.txt",&star);
    printf("carte chargee de taille( %d hauteur , %d largeur ) avec %d voitures et des routes( %d horizontales , %d verticales )\n",parametres_carte.hauteur , parametres_carte.largeur , parametres_carte.nb_voiture , parametres_carte.nb_route_horizontale ,  parametres_carte.nb_route_verticale);
    carte_routiere carte = create_map(parametres_carte);
    //afficher_plateau(carte);
    carte = place_cars_rd(carte , parametres_carte.nb_voiture);
    afficher_plateau(carte);

    //boucle de jeu
    int counter_feux =0;
    while(1){
        if (counter_feux < 4){
            counter_feux++;
        }
        else{
            counter_feux =0;
            carte = feux_change_color(carte);
        }
        for(int i=0;i<carte.nb_voitures;i++){
            carte = move_car(carte , i);
            afficher_plateau(carte);
            Sleep(300);
        }
        Sleep(700);
        printf("\n");
    }

	return 0;
}
