#ifndef LIST_H
#define LIST_H
#include "Outils.h"


typedef struct Noeud Noeud;
struct Noeud {
    Noeud* suivant;
    Message* mex;
};

typedef struct List List;
struct List {
    Noeud* head;
    int size;
};

void printList(List* list);
Message* findMex(List* list, int idImpression);
int extractMex(List* list, int idImpression);
int extract(List* list); //extraction d'un element de la file
Message* getFirst(List* list);
void add(List* list, Message* mexToAdd);
void print(List* list);

#endif
