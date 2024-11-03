#include "utilitaires.c"
pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

 void *lire_clavier(void *arg){
    star_t * star = (star_t *)arg;
    char c;
    while(1) {
        if (_kbhit()) {
            c = _getch();
            pthread_mutex_lock(&dmutex);
            if (c=='z') {
                star->direction = HAUT;
            }else if (c == 's') {
                star->direction = BAS;
            }else if (c == 'd') {
                star->direction = DROITE;
            }else if (c == 'q') {
                star->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutex);
        }
        Sleep(1000);
    }
 }



void calculer_direction(star_t *star) {
    int x=0;
    int y=0;
    switch((*star).direction){
        case HAUT:
            y = -1;
            break;
        case BAS:
            y = 1;
            break;
        case DROITE:
            x = 1;
            break;
        case GAUCHE:
            x = -1;
            break;
    }

    int lig = (*star).posl+y;
    int col = (*star).posc+x;
    printf("(%d,%d)<-(x,y)+(%d;%d)",lig,col,x,y);

    if((*star).plateau[lig][col]!=1){
        (*star).pc = (*star).posc;
        (*star).pl = (*star).posl;

        (*star).posc = col;
        (*star).posl = lig;

        (*star).plateau[lig][col] = 2;
        (*star).plateau[(*star).pl][(*star).pc] = 0;
    }


}
/*
void *deplacer_star(void *arg) {
    star_t * star = (star_t *)arg;
    while(1) {
        pthread_mutex_lock(&dmutex);
        calculer_direction(star);
        // A compléter pour mettre à 0 la case précédente de l'étoile
        // et 1 sa nouvelle position avant d'afficher le plateau
        afficher_plateau(*star);
        pthread_mutex_unlock(&dmutex);
        // Sous windows utiliser Sleep(1000); en millisecondes
        Sleep(1000);
    }
}
*/

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
            Sleep(500);
        }
        printf("\n");
    }
    /*
    placer_star(&star);
    printf("etoile placee\n");
    afficher_plateau(star);
    printf("plateau affiche\n");
    pthread_create(&anim,NULL,deplacer_star,&star);
    pthread_create(&clavier,NULL,lire_clavier,&star);
    pthread_join(anim,NULL);
    */
	return 0;
}
