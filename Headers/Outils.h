#ifndef OUTILS_H
#define OUTILS_H
#include <limits.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#define BUF 1024
#define TRUE 1
#define FALSE 0

struct Message {
    unsigned int lng;
    char type;
    uid_t uid;
    gid_t gid;
    int id_impression;
    char* imprimante;
    char* ref_abs;
    char* tube_client;
};
typedef struct Message Message;

int nombreFichiers(char* fichierConfig);
char** chargeListeFichiers(char* fichierConfig);
char* realpath(const char *restrict file_name, char *restrict resolved_name);
int possedeDroits(Message* mex);
void traiter(mode_t bit, char c1, char c2, int* indice_mode, char* modes);
char *droits(struct stat *pstat);
char* delLastChar(char* string);
char* cheminAbsolu(char* chemin_relatif);
void assigneNombreImpression(Message* mex, int n);
void usageMpr();
void usageInit();
void usageSim();
void usageServer();
Message* buildMessage(char type, char** argv);
void printMex(Message* mex);
unsigned char* structToBytes(Message* msg);
Message* bytesToStruct(unsigned char* arrayBytes);
int endOfFile(char* buffer, int taille);

#endif
