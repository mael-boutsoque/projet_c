#include "jeu1.h"
#include <math.h>
#include <time.h>
#include <stdio.h>

#define Max_number_length 2

int random(int MIN,int MAX){
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    return (rand() % (MAX-MIN+1)) + MIN ;
}

parameters lire_plateau2(char *fichier,star_t *star) {
	FILE *f;
	char str[100];
	int i,j;
	char ch;
    star->lignes = 0;
	f = fopen(fichier,"r");
    if (f == NULL){
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
    int k=0;
	while (fgets(str,sizeof(str),f) != NULL){
		str[strlen(str)-1] = '\0';
		star->colonnes = strlen(str);
		star->lignes++;
	}
	fclose(f);
    char **plateau;
	plateau = (char **)malloc(star->lignes*sizeof(int *));
	for (i=0; i < star->lignes;i++){
		plateau[i] = (char *)malloc(star->colonnes*sizeof(int));
	}

    f = fopen(fichier,"r");
	i = 0; j = 0;
	while ((ch = getc(f)) != EOF) {
        if (ch != '\n') {
            plateau[i][j] = ch-48;
            j++;
        }
        else {
            plateau[i][j] = ch-48;
            j=0;
            i++;
        }
	}

    //convertir en liste de char
    char char_nb[5][Max_number_length+1]; //tableau des nombres sous forme de chars a convertir
    for(int i=0;i<5;i++){
        for(int j=0;j<Max_number_length+1;j++){
            char_nb[i][j] = 'f';
        }
    }
    int ligne = 0; //id de la ligne dans le tableau des chars
    int colonne = 0; //id de la colonne dans le tableau des chars

    for(int i=0;i<3;i++){
        int j=0;
        while(plateau[i][j] != -38){
            if(plateau[i][j] != -16){
                char_nb[ligne][colonne] = plateau[i][j];
                colonne++;
                j++;
            }
            else{
                ligne++;
                colonne = 0;
                j++;
            }
        }
        ligne++;
        colonne = 0;
    }

    int nombres[5];
    //converti en liste de int
    for(int i=0;i<5;i++){
        nombres[i] = 0;
        for(int j=0 ; char_nb[i][j] != 'f' ; j++){
            nombres[i] = nombres[i]*10 + char_nb[i][j];
        }
    }

    parameters carte;
    carte.hauteur = nombres[0];
    carte.largeur = nombres[1];
    carte.nb_voiture = nombres[2];
    carte.nb_route_horizontale = nombres[3];
    carte.nb_route_verticale = nombres[4];

    return carte;
}

carte_routiere create_map(parameters parametres){
    //creer une carte de taille hauteur x largeur
    carte_routiere carte;
    carte.plateau = (int **)malloc(parametres.hauteur*sizeof(int *));
    for (int i=0 ; i < parametres.hauteur ; i++){
		carte.plateau[i] = (int *)malloc(parametres.largeur*sizeof(int));
	}
    carte.lignes = parametres.hauteur;
    carte.colonnes = parametres.largeur;

    /*
    #   0 -> rien
    #   1 -> route -
    #   2 -> route |
    #   3 -> croisement
    #   4 -> feu vert
    #   5 -> feu rouge
    #   6 -> voiture horizontale
    #   7 -> voiture verticale
    */

    for(int i=0;i<carte.lignes;i++){
        for(int j=0;j<carte.colonnes;j++){
            carte.plateau[i][j] = 0;
        }
    }
    for(int i=0;i<carte.lignes;i++){
        for(int j=0;j<carte.colonnes;j++){
            carte.plateau[i][j] = 0;
        }
    }

    //placement des routes

    // //preparation de la liste des feux
    // carte.nb_feux = parametres.nb_route_horizontale*parametres.nb_route_verticale*2;
    // carte.feux = (int **)malloc(carte.nb_feux*sizeof(int *));   
    // for (int i=0 ; i < carte.nb_feux ; i++){
	// 	carte.feux[i] = (int *)malloc(2*sizeof(int));
	// }
    // int id_feux =0; 

    //ecart entre les routes
    int d_route_verticale = carte.lignes / (parametres.nb_route_verticale+1);
    int d_route_horizontale = carte.colonnes / (parametres.nb_route_horizontale+1);
    carte.d_route_verticale = d_route_verticale;
    carte.d_route_horizontale = d_route_horizontale;
    carte.nb_route_horizontale = parametres.nb_route_horizontale;
    carte.nb_route_verticale = parametres.nb_route_verticale;
    printf("ecart routes = (%d , %d)\n", d_route_verticale-1 , d_route_horizontale-1);
    if(d_route_horizontale<3 || d_route_verticale<3){
        printf("!!! ecart trop petit entre les routes (ecart minimum = 2 > ecart acutel = %d) !!!\n",min(d_route_horizontale,d_route_verticale)-1);
    }

    //routes verticales
    for(int i=0;i<parametres.nb_route_verticale;i++){
        for(int j=0;j<carte.colonnes;j++){
            carte.plateau[(i+1)*d_route_verticale][j] = 1;
        }
    }

    // routes horisontales
    for(int i=0;i<carte.lignes;i++){
        for(int j=0;j<parametres.nb_route_horizontale;j++){

            // si c'est un croisement
            if(carte.plateau[i][j] ==1){
                int j2 = (j+1)*d_route_horizontale;
                carte.plateau[i][j2] = 2;

                // //on met des feux
                // carte.plateau[i-2][j2-1] = 5;
                // carte.feux[id_feux][0] = i-2;
                // carte.feux[id_feux][1] = j2-1;
                // id_feux++;
                // carte.plateau[i+1][j2-2] = 4;
                // carte.feux[id_feux][0] = i+1;
                // carte.feux[id_feux][1] = j2-2;
                // id_feux++;

            }

            // si ce n'est pas un croisement
            else{
                carte.plateau[i][(j+1)*d_route_horizontale] = 2;
            }
        }
    }
    return carte;
}

carte_routiere place_cars_rd(carte_routiere carte , int nb_voitures){
    //placement des voitures
    //printf("placement des voitures(nb_voitures=%d , d_route_horizontale=%d , d_route_verticale=%d , nb_route_verticale=%d , nb_route_horizontale=%d)\n",nb_voitures , carte.d_route_horizontale , carte.d_route_verticale , carte.nb_route_verticale , carte.nb_route_horizontale);

    //creer la liste des voitures
    carte.nb_voitures = nb_voitures;
    carte.voitures = (int **)malloc(carte.nb_voitures*sizeof(int *));
    for (int i=0 ; i < carte.nb_voitures ; i++){
		carte.voitures[i] = (int *)malloc(2*sizeof(int));
	}

    //placer les voitures
    for(int id_voit=0 ; id_voit<carte.nb_voitures ; id_voit++){
        int ligne_ou_colonne = random(1,2);

        // on place sur une route horizontale
        if(ligne_ou_colonne ==1){
            int id_route;
            int place;
            do{
                id_route = random(1,carte.nb_route_verticale); //choix de la route
                place = random(0,carte.d_route_horizontale-1); //choix de la place sur la route
            }while(carte.plateau[id_route*carte.d_route_verticale][place] == 6);

            carte.plateau[id_route*carte.d_route_verticale][place] = 6;
            carte.voitures[id_voit][0] = id_route*carte.d_route_verticale;
            carte.voitures[id_voit][1] = place;
        }

        // on place sur une route verticale
        else{
            int id_route;
            int place;
            do{
                id_route = random(1,carte.nb_route_horizontale);//choix de la route
                place = random(0,carte.d_route_verticale-1); //choix de la place sur la route
            }while(carte.plateau[place][id_route*carte.d_route_horizontale] == 7);

            carte.plateau[place][id_route*carte.d_route_horizontale] = 7;
            carte.voitures[id_voit][0] = place;
            carte.voitures[id_voit][1] = id_route*carte.d_route_horizontale;
        }
    }

    //afficher la position des voitures
    /*
    for (int i=0 ; i < carte.nb_voitures ; i++){
		printf("[%d,%d]\n",carte.voitures[i][0],carte.voitures[i][1]);
	}
    */

    return carte;
}

carte_routiere move_car(carte_routiere carte,int id_car){
    if(carte.voitures[id_car] == NULL){
        return carte;
    }
    int y = carte.voitures[id_car][0];
    int x = carte.voitures[id_car][1];

    printf("[%d,%d] ",x,y);

    if(carte.plateau[y][x]==6){
        //deplacement horizontal
        if(carte.voitures[id_car][1]>=carte.colonnes-1){
            carte.voitures[id_car] = NULL;
            carte.plateau[y][x] = 1;
            // carte.plateau[y][x+1] = 6;
        }

        else if((carte.plateau[y][x+1]==1 || carte.plateau[y][x+1]==2) && (x<1 || carte.plateau[y+1][x-1]!=5)){ //test voiture en +1 et test feu rouge
            carte.voitures[id_car][0] = y;
            carte.voitures[id_car][1] = x+1;
            carte.plateau[y][x] = 1;
            carte.plateau[y][x+1] = 6;
            return carte;
        }
        else carte.car_can_move = 0;
    }
    else{
        //deplacement vertical
        if(carte.voitures[id_car][0]>=carte.lignes-1){
            carte.voitures[id_car] = NULL;
            carte.plateau[y][x] = 2;
            //carte.plateau[y+1][x] = 7;
        }

        else if((carte.plateau[y+1][x]==2 || carte.plateau[y+1][x]==1) && (y<1 || carte.plateau[y-1][x-1]!=5)){ //test voiture en +1 et test feu rouge
            carte.voitures[id_car][0] = y+1;
            carte.voitures[id_car][1] = x;
            carte.plateau[y][x] = 2;
            carte.plateau[y+1][x] = 7;
            return carte;
        }
        else carte.car_can_move = 0;
    }
    return carte;
}

carte_routiere feux_change_color(carte_routiere carte){
    for(int i=0;i<carte.nb_feux;i++){
        int y = carte.feux[i][0];
        int x = carte.feux[i][1];
        if(carte.plateau[y][x] ==4){
            carte.plateau[y][x] = 5;
        }
        else{
            carte.plateau[y][x] = 4;
        }
    }
}


void clearScreen()
{
    int n;
    for (n = 0; n < 10; n++)
        printf( "\n\n\n\n\n\n\n\n\n\n" );
}

void afficher_plateau(carte_routiere star) {
    clearScreen();
    int count;
	for(int i=0;i<star.lignes;i++){
        for(int j=0;j<star.colonnes;j++){
            //debug
            count++;
                switch (star.plateau[i][j]){
                case 0:
                    printf(" ");
                    break;
                case 1:
                    printf("-");
                    break;
                case 2:
                    printf("|");
                    break;
                case 3:
                    printf("+");
                    break;
                case 4:
                    printf("V");
                    break;
                case 5:
                    printf("R");
                    break;
                case 6:
                    printf("*");
                    break;
                case 7:
                    printf("*");
                    break;
            }
           //printf("%d|",star.plateau[i][j]);
        }
        printf("\n");
	}
}
