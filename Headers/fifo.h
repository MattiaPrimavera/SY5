#ifndef FIFO_H
#define FIFO_H

#define NMAX 20
#define BUF 1024
#define REQUETE "fifo1" //le tube pour les requetes
#define REPONSE "fifo2" //le tube pour les reponses
#include "List.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct InfoFile{
	int curseur;
	int size;
};
typedef struct InfoFile InfoFile;

struct Imprimante{
	char* nom;
	char* tube;
	List* listeMessages;
	int nombreMessages;
};
typedef struct Imprimante Imprimante;

int send(Message* mex, int fd);
int nombreImprimantes(char* fichierConfig);
Imprimante* extractImprimante(char* toSplit);
Imprimante** chargeImprimantes(char* fichierConfig);
int envoieFichier(Message* mex, int tubeImprimante);

#endif


	