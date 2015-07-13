#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include "../Headers/fifo.h"
#include "../Headers/Outils.h"


int main(int argc, char** argv){
	int nombre = nombreImprimantes(argv[2]);
	pid_t pid;

	//controle sur les arguments du main
	if(strcmp("-c", argv[1]) != 0){
		usageInit();
		exit(-1);
	}

	//chargement liste Imprimantes à travers le fichier de configuration
	Imprimante** listeImprimantes = chargeImprimantes(argv[2]);

	int i; char *args[7];
	args[0] = "sim_impr";

	printf("Demarrage Imprimantes en cours...\n");
	for(i=0;i<nombre;i++){
		switch(pid = fork()){
			case -1:
				perror("Erreur Creation Processus"); exit(2);
			case 0:
				args[1] = "-t";
				args[2] = listeImprimantes[i]->tube; 
				args[3] = "-n";
				args[4] = listeImprimantes[i]->nom;
				args[5] = NULL;
				printf("--> %s  on  %s\n", listeImprimantes[i]->nom, listeImprimantes[i]->tube);
				execvp("./sim_impr", args);
				perror("exec()");
				exit(0);
			default:
				continue;
		}
	}
	return 0;
}
