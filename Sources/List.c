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

//on enqueue le message a la liste
void add(List* list, Message* mexToAdd){
	Noeud* add = malloc(sizeof(Noeud));
	add->mex = mexToAdd;
	add->suivant = NULL;
	if(list->head == NULL) list->head = add;
	else{
		Noeud* tmp = list->head;
		while(tmp->suivant != NULL){
			tmp = tmp->suivant;
		}
		tmp->suivant = add;
	}
	list->size++;
}

Message* findMex(List* list, int idImpression){
	Noeud* tmp = list->head;
	if(list->head == NULL) return NULL;
	else{
		while(tmp != NULL){
			if(tmp->mex->id_impression == idImpression)
				return tmp->mex;
			tmp = tmp->suivant;
		}
	}
	return NULL;
}

void printList(List* list){
	int i;
	if(list->head == NULL){
		printf("LISTE VIDE\n");
		return;
	}
	Noeud* tmp = list->head;
	for(i=0;i<list->size;i++){
		tmp = tmp->suivant;
	}
}

int extractMex(List* list, int idImpression){
	Message* result = findMex(list, idImpression);
	Noeud* tmp = list->head;
	Noeud* precedent;
	if(result == NULL) return -1; //si le message n'est pas dans la liste NULL
	if(tmp->mex->id_impression == idImpression){ //cas tete
		list->head = tmp->suivant;
		list->size--;
		return 0;
	}
	else{ //sinon
		precedent = tmp;
		tmp = tmp->suivant;
		while(tmp != NULL){
			if(tmp->mex->id_impression == idImpression){
				if(tmp->suivant != NULL){
					precedent->suivant = tmp->suivant;
				}
				else
					precedent->suivant = NULL;
				list->size--;
				return 0;
			}
			precedent = tmp;
			tmp = tmp->suivant;
		}
	}
	return -2;
}

//on modifie la tete de la liste puisque on extrait le premier element insere'
int extract(List* list){
	if(list->head == NULL) return -1; //cas liste vide
	list->head = list->head->suivant;
	(list->size)--;
	return 0;
}

Message* getFirst(List* list){
	return list->head->mex;
}

