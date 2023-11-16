#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"
#include <string.h>


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}



int interprete (sequence_t* seq, bool debug)
{
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;
    int a, b, c;
    char *ca = NULL;
    char *cb = NULL;
    char *cc = NULL;
	pile_t *pile = malloc(sizeof(pile_t));
	creer_pile(pile);


    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher(seq);
    printf ("\n");
    if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    int ret;         //utilisée pour les valeurs de retour

    while (prendreTete(seq, &commande)) { //à modifier: condition de boucle

        switch (commande) {
            /* Ici on avance tout le temps, à compléter pour gérer d'autres commandes */

            case 'A':
                ret = avance();
                if (ret == VICTOIRE) return VICTOIRE; /* on a atteint la cible */
                if (ret == RATE)     return RATE;     /* tombé dans l'eau ou sur un rocher */
                break; /* à ne jamais oublier !!! */
            
            case 'D':
                droite();
                break; /* à ne jamais oublier !!! */
            
            case 'G':
                gauche();
                break; /* à ne jamais oublier !!! */
            
            case 'M':
            	ca = depiler(pile, &a);
            	assert(ca == NULL);	// Mesure sur un char* !
        		b = mesure(a);
            	empilerInt(pile, b);
                break; /* à ne jamais oublier !!! */
            
            case 'P':
            	ca = depiler(pile, &a);
            	assert(ca == NULL);	// Pose sur un char* !
                pose(a);
                break; /* à ne jamais oublier !!! */
			
			case '0' ... '9':
				empilerInt(pile, commande - 48);
				break; /* à ne jamais oublier !!! */
			
			case '+':
				ca = depiler(pile, &a);
				cb = depiler(pile, &b);
				assert(ca == NULL && cb == NULL);	// + sur un char* !
				empilerInt(pile, (b + a));
				break; /* à ne jamais oublier !!! */
			
			case '-':
				ca = depiler(pile, &a);
				cb = depiler(pile, &b);
				assert(ca == NULL && cb == NULL);	// - sur un char* !
				empilerInt(pile, (b - a));
				break; /* à ne jamais oublier !!! */
			
			case '*':
				ca = depiler(pile, &a);
				cb = depiler(pile, &b);
				assert(ca == NULL && cb == NULL);	// * sur un char* !
				empilerInt(pile, (b * a));
				break; /* à ne jamais oublier !!! */
			
			case '/':
				ca = depiler(pile, &a);
				cb = depiler(pile, &b);
				assert(ca == NULL && cb == NULL);	// / sur un char* !
				empilerInt(pile, (b / a));
				break; /* à ne jamais oublier !!! */
			
			case '{':
				ca = malloc(sizeof(char) * 512);
				prendreTete(seq, &commande);
				int i;
				int flag = true;
				int count = 0;
				for (i = 0; flag; i++)
				{
					if (commande == '}' && count == 0)
						flag = false;
					else
					{
						if (commande == '{')
							count++;
						else if (commande == '}' && count > 0)
							count--;
						ca[i] = commande;
						prendreTete(seq, &commande);
					}
				}
				ca[i-1] = '\0';
				empilerChar(pile, ca);
				break; /* à ne jamais oublier !!! */
			
			case '?':
				ca = depiler(pile, &a);	// F
				cb = depiler(pile, &b);	// V
				cc = depiler(pile, &c);	// n
				assert(ca != NULL && cb != NULL && cc == NULL);
				if (c == 0)
				{
					conversionTete(ca, seq);
				}
				else
				{
					conversionTete(cb, seq);
				}
				break; /* à ne jamais oublier !!! */
			
			case '!':
				ca = depiler(pile, &a);	// e
				assert(ca != NULL);	// Pas d'execution d'entiers
				conversionTete(ca, seq);
				break; /* à ne jamais oublier !!! */
			
			case 'X':
				ca = depiler(pile, &a);	// a
				cb = depiler(pile, &b);	// b
				empiler(pile, a, ca);
				empiler(pile, b, cb);
				break; /* à ne jamais oublier !!! */
			
			case 'C':
				ca = depiler(pile, &a);	// e
				if (ca == NULL)
				{
					empilerInt(pile, a);
					empilerInt(pile, a);
				}
				else
				{
					empilerChar(pile, ca);
					int size = strlen(ca);
					cb = malloc(sizeof(char) * size);
					strcpy(cb, ca);
					empilerChar(pile, cb);
				}
				break; /* à ne jamais oublier !!! */
			
			case 'I':
				ca = depiler(pile, &a);	// e
				if (ca != NULL)
					free(ca);
				break; /* à ne jamais oublier !!! */

            default:
                eprintf("Caractère inconnu: '%c'\n", commande);
        }

        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        print(pile);
        if (debug) stop();
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    return CIBLERATEE;
}
