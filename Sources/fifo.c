#include "../Headers/fifo.h"
#include "../Headers/Outils.h"
#include "../Headers/fifo.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

int send(Message* mex, int fd)
{
    unsigned char* bytesArray = structToBytes(mex);

    if (write(fd, bytesArray, mex->lng + sizeof(unsigned int)) == -1) {
        perror("write"); return -1;
    }
    return 0;
}

int nombreImprimantes(char* fichierConfig)
{
    char buffer[BUF];
    int fichier = open(fichierConfig, O_RDONLY);
    int lu = 0, lines = 0;

    lu = read(fichier, buffer, sizeof(buffer));
    int i;
    for (i = 0; i < lu; i++) {
        if (buffer[i] == '\n')
            ++lines;
    }
    close(fichier);
    return lines + 1;
}

Imprimante** chargeImprimantes(char* fichierConfig)
{
    int nombreImprimante = nombreImprimantes(fichierConfig);
    Imprimante** listeImprimantes = malloc(nombreImprimante * sizeof(Imprimante*));
    int fichier = open(fichierConfig, O_RDONLY);
    char *token;
    char buffer[2048];
    int lu = 0, i = 0;

    /* get the first token */
    lu = read(fichier, buffer, sizeof(buffer));
    if (lu <= 0) return NULL;
    token = strtok(buffer, "\n");
    /* walk through other tokens */
    while (token != NULL) {
        if (nombreImprimante - 1 == i) {
            char* tokenSansFin = malloc((strlen(token) - 1) * sizeof(char));
            strncpy(tokenSansFin, token, strlen(token) - 1);
            listeImprimantes[i] = extractImprimante(tokenSansFin);
        } else
            listeImprimantes[i] = extractImprimante(token);

        token = strtok(NULL, "\n");
        i++;
    }
    close(fichier);
    return listeImprimantes;
}

Imprimante* extractImprimante(char* toSplit)
{
    Imprimante* impr = malloc(sizeof(Imprimante));
    int i;

    for (i = 0; i < strlen(toSplit); i++) {
        if (toSplit[i] == ' ') {
            impr->nom = malloc(i * sizeof(char));
            strncpy(impr->nom, toSplit, i);
            impr->tube = malloc((strlen(toSplit) - i - 1) * sizeof(char));
            strncpy(impr->tube, toSplit + i + 1, (strlen(toSplit) - i - 1));
            impr->listeMessages = malloc(sizeof(List));
            impr->listeMessages->head = NULL;
            impr->listeMessages->size = 0;
            impr->nombreMessages = 0;
            break;
        }
    }
    return impr;
}

int envoieFichier(Message* mex, int tubeImprimante)
{
    int rc, wc;
    int i = 0;
    int fichier_lire;
    char buf [BUF];
    struct stat status;

    //controle existance fichier et ouverture tube en lecture
    if (stat(mex->ref_abs, &status) == -1) {
        perror("stat");
        return(EXIT_FAILURE);
    }
    fichier_lire = open(mex->ref_abs, O_RDONLY);
    if (fichier_lire == -1) {
        fprintf(stderr, "ouverture des tubes impossible\n"); exit(2);
    }

    //on parcour le fichier en envoyant la totalite'
    while (i < status.st_size) {
        rc = read(fichier_lire, buf, BUF);
        if (rc < 0) {
            perror("read");
            return -1;
        }       else if (rc == 0)
            break;
        else { //on a lu quelque chose, on l'envoie
            wc = write(tubeImprimante, buf, rc);
            if (wc < 0) {
                perror("write");
                return -1;
            }
            i += rc;
        }
    }
    //envoie caracteres terminaison fichier
    wc = write(tubeImprimante, "0000000000", 10);
    if (wc <= 0) {
        perror("write");
        return -1;
    }

    close(fichier_lire);
    return 0;
}

InfoFile* chargeInfoFile(Message* mex)
{
    struct stat fichier;

    if (stat(mex->ref_abs, &fichier) == -1) {
        return NULL;
    }
    InfoFile* result = malloc(sizeof(InfoFile));
    result->size = (int)fichier.st_size;
    result->curseur = 0;
    return result;
}
