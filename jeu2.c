#include "utilitaires2.c"

void *move_car(void *arg){
    thread_parameters * parametres = (thread_parameters*) arg;
    int id_car = parametres->id;
    int delay = 500+random(0,700);
    printf("[voiture %d]\n",id_car);
    while(1){
        int y = parametres->carte.voitures[id_car][0];
        int x = parametres->carte.voitures[id_car][1];

        pthread_mutex_lock(&dmutex);
        //printf("[%d,%d]\n",x,y);

        if(parametres->carte.plateau[y][x]==6){
            //deplacement horizontal
            if(parametres->carte.voitures[id_car][1] > parametres->carte.colonnes - 2){
                parametres->carte.voitures[id_car][0] = y;
                parametres->carte.voitures[id_car][1] = 0;
                parametres->carte.plateau[y][x] = 1;
                parametres->carte.plateau[y][0] = 6;
            }
            else if((parametres->carte.plateau[y][x+1]==1 || parametres->carte.plateau[y][x+1]==2) && (x<1 || parametres->carte.plateau[y+1][x-1]!=5)){ //test voiture en +1 et test feu rouge
                parametres->carte.voitures[id_car][0] = y;
                parametres->carte.voitures[id_car][1] = x+1;
                parametres->carte.plateau[y][x] = 1;
                parametres->carte.plateau[y][x+1] = 6;
            }
        }
        else{
            //deplacement vertical
            if(parametres->carte.voitures[id_car][0] > parametres->carte.lignes - 2){
                parametres->carte.voitures[id_car][0] = 0;
                parametres->carte.voitures[id_car][1] = x;
                parametres->carte.plateau[y][x] = 2;
                parametres->carte.plateau[0][x] = 7;
            }
            else if((parametres->carte.plateau[y+1][x]==2 || parametres->carte.plateau[y+1][x]==1) && (y<1 || parametres->carte.plateau[y-1][x-1]!=5)){ //test voiture en +1 et test feu rouge
                parametres->carte.voitures[id_car][0] = y+1;
                parametres->carte.voitures[id_car][1] = x;
                parametres->carte.plateau[y][x] = 2;
                parametres->carte.plateau[y+1][x] = 7;
            }
        }
        pthread_mutex_unlock(&dmutex);
        Sleep(delay);
    }
}

void *feux_loop(void *arg){ //les feux ne sont pas liés avec leur feu opposé pour tester les colisions dans un carrefour
    thread_parameters * parametres = (thread_parameters*) arg;
    int id_feu = parametres->id;
    int y = parametres->carte.feux[id_feu][0];
    int x = parametres->carte.feux[id_feu][1];
    int delay = 3000 + random(0,2000);
    printf("[feux %d]\n",id_feu);
    while(1){
        Sleep(delay);
        if(parametres->carte.plateau[y][x]==4){
            parametres->carte.plateau[y][x] = 5;
        }
        else{
            parametres->carte.plateau[y][x] = 4;
        }
    }
}

int main() {
    star_t star;
    srand(time(0));
	parameters parametres_carte = lire_plateau2("plateau2.txt",&star);
    printf("carte chargee de taille( %d hauteur , %d largeur ) avec %d voitures et des routes( %d horizontales , %d verticales )\n",parametres_carte.hauteur , parametres_carte.largeur , parametres_carte.nb_voiture , parametres_carte.nb_route_horizontale ,  parametres_carte.nb_route_verticale);
    carte_routiere carte = create_map(parametres_carte);
    //afficher_plateau(carte);
    carte = place_cars_rd(carte , parametres_carte.nb_voiture);
    //afficher_plateau(carte);


    /// create threads
    // affichage
    pthread_t anim;
    pthread_create(&anim , NULL , afficher_plateau , &carte);

    //voitures
    int nb_voitures = carte.nb_voitures;
    thread_parameters *parametres_voitures = malloc(sizeof(thread_parameters) * nb_voitures);
    pthread_t threads_voitures[nb_voitures];

    for(int i=0;i<nb_voitures;i++){
        parametres_voitures[i].carte = carte;
        parametres_voitures[i].id = i;
        pthread_create(&threads_voitures[i] , NULL , move_car , &parametres_voitures[i]);
    }

    //feux tricolores
    
    int nb_feux = carte.nb_feux;
    thread_parameters *parametres_feux = malloc(sizeof(thread_parameters) * nb_feux);
    pthread_t threads_feux[nb_feux];

    for(int i=0;i<nb_feux;i++){
        parametres_feux[i].carte = carte;
        parametres_feux[i].id = i;
        pthread_create(&threads_feux[i] , NULL , feux_loop , &parametres_feux[i]);
    }


    /// join threads
    //voitures
    for(int i=0;i<nb_voitures;i++){
        pthread_join(threads_voitures[i],NULL);
    }
    free(parametres_voitures);

    //feux tricolores
    for(int i=0;i<nb_feux;i++){
        pthread_join(threads_feux[i],NULL);
    }
    free(parametres_feux);

    // affichage
    pthread_join(anim,NULL);
    printf("thread anim joined\n");

	return 0;
}
