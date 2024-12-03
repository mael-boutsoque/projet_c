#include <stdio.h>
#include <stdlib.h>

int main()
{
    int choix;

    do
    {
        printf("1 pour la version normale, 2 pour la version avec threads : ");
        scanf("%d", &choix);

        if (choix != 1 && choix != 2)
        {
            printf("Erreur : Veuillez entrer 1 ou 2.\n");
        }
    } while (choix != 1 && choix != 2);

    switch(choix){
        case 1:
        printf("version normale\n");
        system("C:/Users/maelb/Documents/ecole/C/projet_c/projet_c/jeu.exe");
        break;

        case 2:
        printf("version avec threads\n");
        system("./projet_c_multi_thread/jeu.exe");
        break;
    }


    return 0;
}