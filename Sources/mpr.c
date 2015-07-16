//usage: mpr imprimante fichier
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
#include "../Headers/mpr.h"

//usage ./mpr -P imprimante fichier tube (-A)
int main(int argc, char** argv){
	int desc_requete, desc_reponse; //descripteurs des tubes nommees
	unsigned char* buffer = malloc(BUF*sizeof(unsigned char));
	Message* message;
	struct stat fichier;
	if(argc != 5 && argc != 7){ //wrong args number
		usageMpr();
		exit(-2);
	}
	if(strcmp(argv[1], "-P") != 0){ //missing P option
		usageMpr();
		exit(-1);
	}

	if(argc == 7 && strcmp(argv[5], "-N") == 0){
		printf("siamo nell'opzione N");
		int nombre = atoi(argv[6]);
		printf("%d --> nombre\n", nombre);
		pid_t pid;

		int i; char *args[7];
		args[0] = "./mpr";
		printf("Demarrage Imprimantes en cours...\n");
		for(i=0;i<nombre;i++){
			switch(pid = fork()){
				case -1:
					perror("Erreur Creation Processus"); exit(2);
				case 0: //      ./mpr -P francino fichier tubeClient
					args[1] = "-P";
					args[2] = argv[2]; 
					args[3] = argv[3];
					args[4] = argv[4];
					args[5] = NULL;
					printf("--> ./mpr -P %s %s %s\n", args[2], args[3], args[4]);
					execvp("./mpr", args);
					perror("exec()");
					exit(0);
				default:
					continue;
			}
		}
		exit(0);
	}

	if(argc == 7 && strcmp(argv[5], "-M") == 0){
		int nombre = nombreFichiers(argv[6]);
		pid_t pid;		

		//chargement liste Imprimantes à travers le fichier de configuration
		char** listeFichiers = malloc(nombre*sizeof(char*));
		listeFichiers = chargeListeFichiers(argv[6]);	

		int i; char *args[7];
		args[0] = "mpr";
		for(i=0;i<nombre;i++)
			printf("%s\n", listeFichiers[i]);

		printf("Demarrage Imprimantes en cours...\n");
		for(i=0;i<nombre;i++){
			switch(pid = fork()){
				case -1:
					perror("Erreur Creation Processus"); exit(2);
				case 0:
					args[1] = "-P";
					args[2] = argv[2]; 
					args[3] = listeFichiers[i];
					args[4] = argv[3];
					args[5] = NULL;
					printf("--> ./mpr -P %s %s %s\n", args[2], listeFichiers[i], args[4]);
					execvp("./mpr", args);
					perror("exec()");
					exit(0);
				default:
					continue;
			}
		}
	}	

	//verification fichier existe et regulier
	if(stat(argv[3], &fichier) == -1){ //cas erreur ouverture, donc fichier inexistant aussi
		perror("erreur ouverture fichier...");
		exit(-1); 
    }
    if(!S_ISREG(fichier.st_mode)){
    	perror("error: fichier non regulier");
    	exit(-1);
   	}


	//les droits d'acces sur les tubes seront rw pour tous
	mode_t mode = S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;

	//Creation tube client pour recevoir reponse serveur
	if(mkfifo(argv[4], mode) == -1){
		perror("error : (tubeClient) ");
	}

	char* variable = getenv("IMP_PATH");
	desc_requete = open(variable, O_WRONLY);
	if(desc_requete == -1){
		fprintf(stderr, "error: ouverture des tubes impossible (IMP_PATH not set)	\n"); exit(2);
	} 
	
	
	//construction message
	message = buildMessage('i', argv);
	if(argc == 7 && strcmp(argv[5], "-A") == 0){
		printf("opzione a inserita");
		message->type = 'a';
		message->id_impression = atoi(argv[6]);
	}
	//envoie de la requete
	unsigned char* bytesArray = structToBytes(message);
	printMex(bytesToStruct(bytesArray));

	if(write(desc_requete, bytesArray, message->lng + sizeof(unsigned int)) == -1){
		perror("write"); exit(2);
	}

	printf("\n*** Message envoyé avec success ***\n");

	//attente reponse serveur
	desc_reponse = open(argv[4], O_RDONLY);
	if(desc_reponse == -1){
		fprintf(stderr, "ouverture des tubes impossible\n"); exit(2);
	} 	
	int nb_lu = read(desc_reponse, buffer, BUF);

	if (nb_lu < 0){
		perror("read");
	}else{
		message = bytesToStruct(buffer);
		switch(message->id_impression){
			case -2:
				printf("error: imprimante demandee' n'existe pas ...\n ");
				break;
			case -1: 
				printf("error: pas de droits sur le fichier demande'\n");
				break;
			default: 
				printMex(message);
				break;
		}	

	}
		
	return 0;
}

