#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <errno.h>

#include "../Headers/List.h"
#include "../Headers/Outils.h"
#include "../Headers/fifo.h"

/* usage: server */
int main(int argc, char** argv)
{

    if (argc != 5) {
        usageServer();
        exit(1);
    }

    Message* message;
    int nombre_impression = 1000;
    unsigned char buffer[200];
    int desc_requete;
    int nombreImprimante;
    Imprimante** listeImprimantes;
    int* descripteursImprimantes;

    if (strcmp(argv[1], "-t") == 0 && strcmp(argv[3], "-c") == 0) {
        //les droits d'acces sur les tubes seront rw pour tous
        mode_t mode = S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;
        //une fois lance', le serveur cree les deux tubes
        if (mkfifo(argv[2], mode) == -1) {
            fprintf(stderr, "creation des tubes impossible\n");
            exit(2);
        }

        desc_requete = open(argv[2], O_RDONLY | O_NONBLOCK);
        if (desc_requete == -1) {
            perror("open()");
            exit(EXIT_FAILURE);
        }

        //verification existance fichier configuration
        struct stat fichierConfiguration;
        if (stat(argv[4], &fichierConfiguration) == -1) {
            perror("");
            exit(-1);
        }
        if (!S_ISREG(fichierConfiguration.st_mode)) {
            perror("fichier non regulier");
            exit(-1);
        }

        //chargement liste Imprimantes à travers le fichier de configuration
        nombreImprimante = nombreImprimantes(argv[4]);
        listeImprimantes = chargeImprimantes(argv[4]);
        if (listeImprimantes == NULL) {
            perror("format fichier configuration");
            exit(-1);
        }
        //creation de tubes de communication avec les imprimantes
        descripteursImprimantes = malloc(nombreImprimante * sizeof(int));
        int i;
        for (i = 0; i < nombreImprimante; i++) {
            descripteursImprimantes[i] = open(listeImprimantes[i]->tube, O_WRONLY | O_NONBLOCK);
        }
    } else if (strcmp(argv[1], "-c") == 0 && strcmp(argv[3], "-t") == 0) {
        //les droits d'acces sur les tubes seront rw pour tous
        mode_t mode = S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;
        //une fois lance', le serveur cree les deux tubes
        if (mkfifo(argv[4], mode) == -1) {
            fprintf(stderr, "creation des tubes impossible\n");
            exit(2);
        }

        desc_requete = open(argv[4], O_RDONLY | O_NONBLOCK);
        if (desc_requete == -1) {
            perror("open()");
            exit(EXIT_FAILURE);
        }

        //verification existance fichier configuration
        struct stat fichierConfiguration;
        if (stat(argv[2], &fichierConfiguration) == -1) {
            perror("");
            exit(-1);
        }
        if (!S_ISREG(fichierConfiguration.st_mode)) {
            perror("fichier non regulier");
            exit(-1);
        }

        //chargement liste Imprimantes à travers le fichier de configuration
        nombreImprimante = nombreImprimantes(argv[2]);
        listeImprimantes = chargeImprimantes(argv[2]);
        if (listeImprimantes == NULL) {
            perror("format fichier configuration");
            exit(-1);
        }
        //creation de tubes de communication avec les imprimantes
        descripteursImprimantes = malloc(nombreImprimante * sizeof(int));
        int i;
        for (i = 0; i < nombreImprimante; i++) {
            descripteursImprimantes[i] = open(listeImprimantes[i]->tube, O_WRONLY | O_NONBLOCK);
        }
    } else {
        usageServer();
        exit(-1);
    }

    while (TRUE) {
        //ETAPE 1 - on verifie la presence d'une commande
        int count = read(desc_requete, buffer, 200);
        if (count <= 0) {
            //tour imprimantes
            int i;
            for (i = 0; i < nombreImprimante; i++) {
                if (listeImprimantes[i]->listeMessages->size > 0) {
                    if (envoieFichier(getFirst(listeImprimantes[i]->listeMessages), descripteursImprimantes[i]) == -1) {
                        printf("Erreur: Envoie fichier impossible...\n");
                        continue;
                    } else {
                        extract(listeImprimantes[i]->listeMessages);
                    }
                } else
                    continue;

            }
        } else if (count > 0) {
            //cas reception message
            message = bytesToStruct(buffer);
            printMex(message);
            int boleano;
            int desc_client = open(message->tube_client, O_WRONLY);

            //interpretation selon le type
            switch (message->type) {
            case 'i':
                //Reponse au client avec ID Impression
                nombre_impression++;
                if (possedeDroits(message) == 0)
                    assigneNombreImpression(message, nombre_impression);
                else {
                    assigneNombreImpression(message, -1);
                    send(message, desc_client);
                    continue;
                }

                //on cherche la bonne liste et on enchaine le message
                int i, bool2 = FALSE;
                for (i = 0; i < nombreImprimante; i++) {
                    if (strcmp(listeImprimantes[i]->nom, message->imprimante) == 0) {
                        add(listeImprimantes[i]->listeMessages, message);
                        bool2 = TRUE;
                        break;
                    }
                }

                if (bool2 == FALSE) {
                    message->id_impression = -2;
                    send(message, desc_client);
                    continue;
                }

                printMex(message);
                send(message, desc_client);
                //on affiche la requete d'impression
                printf("UID = %d; GID = %d; ID = %d; FIC = \"%s\"\n", message->uid, message->gid, message->id_impression, message->ref_abs);

                memset(buffer, '\0', 200);

                break;
            case 'a':
                boleano = FALSE;
                Message* toSend;
                for (i = 0; i < nombreImprimante; i++) {
                    if ((toSend = findMex(listeImprimantes[i]->listeMessages, message->id_impression)) != NULL) {
                        toSend->type = 'a';
                        printMex(toSend);
                        send(toSend, desc_client);
                        boleano = TRUE;
                        if (extractMex(listeImprimantes[i]->listeMessages, message->id_impression) == -1)
                            printf("Erreur extraction Message!\n");
                    }
                }
                if (!boleano) {
                    printf("Le message n'a pas ete' trouve'!\n");
                    send(message, desc_client);
                }

                break;
            }
        }

    } //fin while
}
