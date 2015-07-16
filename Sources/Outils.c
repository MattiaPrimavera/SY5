#include "../Headers/Outils.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

void usageServer(){
	printf("Usage Server :\n./print_demon -t tubeServer -c fichierConfiguration\n(L'ordre des options n'est pas important)\n");
}
void usageMpr(){
	printf("Usage Mpr (Client Impression):\n./mpr -P imprimante fichier tubeClient\n");
}
void usageSim(){
	printf("Usage Sim (Simulateur Impression):\n./sim_impr -t tube_in -n nom\n");
}
void usageInit(){
	printf("Usage Init (Initialisateur Simulateurs):\n./init_simulateurs -c fichierConfiguration\n");
}

int possedeDroits(Message* mex){
	struct stat fichier;
	if(stat(mex->ref_abs, &fichier) == -1){
		perror("stat");
		exit(-1);
	}
	char* droitsFichier = malloc(10);
	droitsFichier = droits(&fichier);
  
  if (fichier.st_uid == mex->uid){
    if(droitsFichier[0] == 'r')
      return (0);
  }

  if (fichier.st_gid == mex->gid){
    if (droitsFichier[3] == 'r')
      return (0);
  }
  
  else{
    if (droitsFichier[6] == 'r')
      return(0);
  }

  return (-1);
}

char *droits(struct stat *pstat){
  mode_t mode;
  char* modes = malloc(sizeof(10));
  int indice_mode = 0;

  mode = pstat->st_mode;

  traiter(mode & S_IRUSR, 'r', '-', &indice_mode, modes);
  traiter(mode & S_IWUSR, 'w', '-', &indice_mode, modes);
  traiter(mode & S_IXUSR, 'x', '-', &indice_mode, modes);
  traiter(mode & S_IRGRP, 'r', '-', &indice_mode, modes);
  traiter(mode & S_IWGRP, 'w', '-', &indice_mode, modes);
  traiter(mode & S_IXGRP, 'x', '-', &indice_mode, modes);
  traiter(mode & S_IROTH, 'r', '-', &indice_mode, modes);
  traiter(mode & S_IWOTH, 'w', '-', &indice_mode, modes);
  traiter(mode & S_IXOTH, 'x', '-', &indice_mode, modes);
  modes[indice_mode]='\0';
  return modes;
}

void traiter(mode_t bit, char c1, char c2, int* indice_mode, char* modes){
    modes[(*indice_mode)++] = bit ? c1:c2;
}

char* cheminAbsolu(char* chemin_relatif){
    char* result = malloc(200*sizeof(char));
    chdir(chemin_relatif);
    getcwd(result, 200);     
    return result;
}

char* delLastChar(char* string){
	int i;
	char* result = malloc(strlen(string)*sizeof(char)+1);
	for(i=0;i<strlen(string)-1;i++)
		result[i] = string[i];
	result[strlen(string)-1] = '\0';
	return result;
}
//Construction du Message d'impression
Message* buildMessage(char type, char** argv){
	Message* message = malloc(sizeof(Message));
	message->type = type;
	message->uid = getuid();
	message->gid = getgid();
	message->imprimante = malloc(strlen(argv[2])+1);
	strcpy(message->imprimante, argv[2]);

	char* absolute_path = realpath(argv[3], NULL);
	message->ref_abs = malloc(strlen(absolute_path)+1);
	strcpy(message->ref_abs, absolute_path);
	message->tube_client = malloc(strlen(argv[4])+1);
	strcpy(message->tube_client, argv[4]);
	message->id_impression = 0;
	message->lng = sizeof(message->type) + sizeof(message->uid)+sizeof(message->gid)+sizeof(message->id_impression)+strlen(message->imprimante)+1+strlen(message->ref_abs)+1+strlen(message->tube_client)+1;	
	return message;
}

void assigneNombreImpression(Message* mex, int n){
	mex->id_impression = n;
}

unsigned char* structToBytes(Message* msg){
	int tailleArray = sizeof(unsigned int) + msg->lng;
	unsigned char* arrayBytes = malloc(tailleArray);
	unsigned char* ptr = arrayBytes;
	//copie champs lng
	memcpy(ptr, &(msg->lng), sizeof(unsigned int));
	ptr = ptr + sizeof(unsigned int);
	//copie de type
	memcpy(ptr, &(msg->type), sizeof(char));
	ptr = ptr + sizeof(char);
	//copie de uid
	memcpy(ptr, &(msg->uid), sizeof(uid_t));
	ptr = ptr + sizeof(uid_t);
	//copie de gid
	memcpy(ptr, &(msg->gid), sizeof(gid_t));
	ptr = ptr + sizeof(gid_t);
	//copie de idImprimante
	memcpy(ptr, &(msg->id_impression), sizeof(int));
	ptr = ptr + sizeof(int);
	//copie de imprimante
	memcpy(ptr, msg->imprimante, strlen(msg->imprimante));
	ptr = ptr + strlen(msg->imprimante);
	*ptr = '\0'; ptr++;
	//copie de ref_abs
	memcpy(ptr, msg->ref_abs, strlen(msg->ref_abs));
	ptr = ptr + strlen(msg->ref_abs);
	*ptr = '\0'; ptr++;
	//copie de tube_client
	memcpy(ptr, msg->tube_client, strlen(msg->tube_client));
	ptr = ptr + strlen(msg->tube_client);
	*ptr = '\0';

	return arrayBytes;
}

Message* bytesToStruct(unsigned char* arrayBytes){
	Message* msg = malloc(sizeof(Message));
	unsigned char* ptr = arrayBytes;
	//copie champs lng
	memcpy(&(msg->lng), ptr, sizeof(unsigned int));
	ptr = ptr + sizeof(unsigned int);
	//copie de type
	memcpy(&(msg->type), ptr, sizeof(char));
	ptr = ptr + sizeof(char);
	//cope de uid
	memcpy(&(msg->uid), ptr, sizeof(uid_t));
	ptr = ptr + sizeof(uid_t);
	//copie de gid
	memcpy(&(msg->gid), ptr, sizeof(gid_t));
	ptr = ptr + sizeof(gid_t);
	//copie id_impression
	memcpy(&(msg->id_impression), ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	//copie de imprimante
	int tailleString = 0;
	unsigned char* ptr2 = ptr;
	while(*ptr2 != '\0'){
		tailleString++;
		ptr2++;
	}
	char* impr = malloc((tailleString+1)*sizeof(char));
	memcpy(impr, ptr, tailleString+1);

	msg->imprimante = impr;
	//copie ref_abs
	tailleString = 0;
	ptr2++; //on se met sur le premier caractere de ref_abs
	ptr = ptr2;
	while(*ptr != '\0'){
		tailleString++;
		ptr++;
	}
	char* ref = malloc((tailleString+1)*sizeof(char));
	memcpy(ref, ptr2, tailleString+1);
	msg->ref_abs = ref;
	//copie de tube_client
	tailleString = 0;
	ptr++;
	ptr2 = ptr;
	while(*ptr2 != '\0'){
		tailleString++;
		ptr2++;
	}
	char* tubestr = malloc((tailleString+1)*sizeof(char));
	memcpy(tubestr, ptr, tailleString+1);
	msg->tube_client = tubestr;
	return msg;
}

void printMex(Message* mex){
	printf("*************MESSAGE*************\n");
	printf("mex.lng --> %u\n", mex->lng);
	printf("mex.type --> %c\n", mex->type);
	printf("mex.uid --> %d\n", mex->uid);
	printf("mex.gid --> %d\n", mex->gid);
	printf("mex.imprimante --> %s\n", mex->imprimante);
	printf("mex.ref_abs --> %s\n", mex->ref_abs);
	printf("mex.tube_client --> %s\n", mex->tube_client);
	printf("mex.id_impression --> %d\n", mex->id_impression);
}
int endOfFile(char* buffer, int taille){
	int i;
	int count = 0;
	//on parcour le buffer
	for(i=0; i<taille; i++){
		if(buffer[i] == '0'){ 
			count++;
		}
		//s'on a trouve' dis \0 on est arrive a la fin du fichier
		if(count == 10) return TRUE;
		if(buffer[i] != '0')
			count = 0;
	}
	return FALSE;
}

int nombreFichiers(char* fichierConfig){
	char buffer[BUF];
	int fichier = open(fichierConfig, O_RDONLY);
	int lu = 0, lines = 0;
	lu = read(fichier, buffer, sizeof(buffer));
	int i;
    for (i=0; i<lu; i++){
     	if (buffer[i] == '\n')
          	++lines;
    }
	
	close(fichier);
	return lines+1;
}

char** chargeListeFichiers(char* fichierConfig){
	int nombre = nombreFichiers(fichierConfig);
	char** listeFichiers = malloc(nombre*sizeof(char*));
	
	int fichier = open(fichierConfig, O_RDONLY);
	if(fichier == -1){
		perror("fichier");
		exit(-1);
	}
	
	int indiceFichiers = 0;
	char buffer[BUF];
	int lu = 0;
	lu = read(fichier, buffer, sizeof(buffer));
	char* fin;

	int i;
	char* precedent = buffer-1;
	char* tmp = malloc(20*sizeof(char));
    for (i=0; i<lu; i++){
    	if (buffer[i] == '\n'){
    		strncpy(tmp, precedent+1, (int)(&buffer[i]-precedent));
    		tmp[(int)(&buffer[i]-precedent)-1] = '\0';
    		listeFichiers[indiceFichiers] = malloc((strlen(tmp)+1)*sizeof(char));
    		strcpy(listeFichiers[indiceFichiers], tmp);
    	   	indiceFichiers++;
    	   	precedent = &buffer[i];
    	   	fin = &buffer[i]+1;
    	}
	}
	buffer[lu] = '\0';
	strncpy(tmp, fin, (int)(&buffer[lu]-fin+1));
	listeFichiers[indiceFichiers] = malloc((strlen(tmp)+1)*sizeof(char));
	strcpy(listeFichiers[indiceFichiers], tmp);
	close(fichier);
	return listeFichiers;
}
