#include "../Headers/Outils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define NMAX 20
#define REQUETE "fifo1" //le tube pour les requetes
#define REPONSE "fifo2" //le tube pour les reponses
#define BUF 1024


int main(int argc, char *argv[]){
	int lu = 0, ecrit = 0;
	int tubeEntree, tubeSortie;
	char buf [1024];
  int nombreImpression = 0;

  //control arguments
  if(((strcmp(argv[1], "-t") != 0) || (strcmp(argv[3],"-n")!= 0))){
    usageSim();
    exit(-1);
  }

  //ouverture et configuration tubes
	mode_t mode = S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH; //droits rw pour tous
  //Creation Tube Impression, si il existe deja, alors on l'utilisera
	if(mkfifo(argv[2], mode) == -1){
    perror("creation tubes"); exit(-1);
  } 
	tubeSortie = open("/dev/null", O_WRONLY); 
	tubeEntree = open(argv[2], O_RDONLY);
	if(tubeEntree == -1 || tubeSortie == -1){
		perror("open fichier_lire");
        exit(0);
	}

	while(TRUE){
   		lu = read(tubeEntree, buf, BUF);
   		if(lu>0)
   		{	
        //on verifie s'on a recu un fichier complet
   			int ris =endOfFile(buf,lu);
	   			if( ris == TRUE){
           nombreImpression++;
    			 printf("%s -> N^%d: Impression en cours...", argv[4], nombreImpression);
    			 sleep(3); 
    			 printf(" --> DONE\n");
    			}
   			ecrit = write(tubeSortie, buf, lu);

    	if (lu == -1 || ecrit == -1){
          perror("write"); close(tubeEntree); close(tubeSortie); exit(EXIT_FAILURE);
        }		
    	}
 	}   	
}
