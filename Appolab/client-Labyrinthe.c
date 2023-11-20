#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAXMSG MAXREP

int main() {

    
    char reponse[MAXREP]; // pour stocker la réponse du serveur

    // Affiche les échanges avec le serveur (false pour désactiver)
    mode_debug(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr", 443);
    // utilisez le port 443 en cas de problème sur le 9999
    /* connexion("im2ag-appolab.u-ga.fr", 443); */

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer_recevoir("login NUMERO \"LOGIN\"", reponse);
    envoyer_recevoir("load Labyrinthe", reponse);
    // Notez qu'ici on n'utilise pas la reponse du serveur

    envoyer_recevoir("start", reponse);
    envoyer_recevoir("{3M{1M{7M{DD}{G}?}{}?}{D}?AC!}C!", reponse);

    printf ("Fin de la connection au serveur\n");
    return 0;
}
