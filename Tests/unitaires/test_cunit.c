 /*
 *           CUnit : A Unit testing framework for C.
 *           http://cunit.sourceforge.net/
 *
 *       --Run Summary: Type      Total     Ran  Passed  Failed
 *                      suites        1       1     n/a       0
 *                      tests         2       2       2       0
 *                      asserts       5       5       5       0
 */


#include <stdio.h>
#include <string.h>
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
#include "CUnit/Headers/Basic.h"

struct InfoFile{
   int curseur;
   int size;
};
typedef struct InfoFile InfoFile;

struct Message{
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

typedef struct Noeud Noeud;
struct Noeud{
   Noeud* suivant;
   Message* mex;
};

typedef struct List List;
struct List{
   Noeud* head;
   int size;
};


struct Imprimante{
   char* nom;
   char* tube;
   List* listeMessages;
   int nombreMessages;
};
typedef struct Imprimante Imprimante;

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
   if (NULL == (temp_file = fopen("Objects/temp.txt", "w"))) {
      return -1;
   }
   else {
      return 0;
   }
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
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

int envoieFichier(Message* mex, int tubeImprimante){
   int rc, wc;
   int i = 0;
   int fichier_lire; 
   char buf [1024];
   struct stat status;

   //controle existance fichier et ouverture tube en lecture
   if(stat(mex->ref_abs, &status) == -1){
      perror("stat");
      return(EXIT_FAILURE);
   }
   fichier_lire = open(mex->ref_abs, O_RDONLY);
   if(fichier_lire == -1){
      fprintf(stderr, "ouverture des tubes impossible\n"); exit(2);
   }

   //on parcour le fichier en envoyant la totalite'
   while(i < status.st_size){
      rc = read(fichier_lire, buf, 1024);
      //printf("%d caracteres lus du fichier : %s\n", rc, mex->ref_abs);
      if(rc < 0){
            perror("read");
            return -1;
      }
         else if(rc == 0)
            break;
         else{//on a lu quelque chose, on l'envoie
            wc = write(tubeImprimante, buf, rc);
            //printf("%d <-- descrittore tube", tubeImprimante);
            //printf("scrittura caratteri terminazione\n");
         if(wc < 0){
               perror("write");
               return -1;
         }
         i += rc;
      } 
   }
   //envoie caracteres terminaison fichier
   wc = write(tubeImprimante, "0000000000", 10);
   if (wc <= 0){
      perror("write");
      return -1;
    }
  
   close(fichier_lire);
   return 0;
}

Imprimante* extractImprimante(char* toSplit){
   if(strlen(toSplit) == 0) return NULL;

   Imprimante* impr = malloc(sizeof(Imprimante));
   int i;
   for(i=0;i<strlen(toSplit);i++){
      if(toSplit[i] == ' '){
         impr->nom = malloc(i*sizeof(char));
         strncpy(impr->nom, toSplit, i);
         impr->tube = malloc((strlen(toSplit)-i-1)*sizeof(char));
         strncpy(impr->tube, toSplit+i+1, (strlen(toSplit)-i-1));
      // printf("modifica\n");
         impr->listeMessages = malloc(sizeof(List));
         impr->listeMessages->head = NULL;
         impr->listeMessages->size = 0;
         impr->nombreMessages = 0;
         break;
      }
   }
   return impr;
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
      //printf("%c_", *ptr2);
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
      //printf("%c_", *ptr);
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
      //printf("%c_", *ptr);
      tailleString++;
      ptr2++;
   }
   char* tubestr = malloc((tailleString+1)*sizeof(char));
   memcpy(tubestr, ptr, tailleString+1);
   msg->tube_client = tubestr;
   return msg;
}

int send(Message* mex, int fd){
   unsigned char* bytesArray = structToBytes(mex);
   if(write(fd, bytesArray, mex->lng + sizeof(unsigned int)) == -1){
      perror("write"); return -1;            
   }
   return 0;
}

int nombreImprimantes(char* fichierConfig){
   char buffer[1024];
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

InfoFile* chargeInfoFile(Message* mex){
   struct stat fichier;
   if(stat(mex->ref_abs, &fichier) == -1){
      return NULL;
   }
   InfoFile* result = malloc(sizeof(InfoFile));
   result->size = (int)fichier.st_size;
   result->curseur = 0;
   return result;
}

Imprimante** chargeImprimantes(char* fichierConfig){
   int nombreImprimante = nombreImprimantes(fichierConfig);
   Imprimante** listeImprimantes = malloc(nombreImprimante*sizeof(Imprimante*));
   int fichier = open(fichierConfig, O_RDONLY);
   char *token;
   char buffer[2048];
   int lu = 0, i = 0;
   /* get the first token */
   lu = read(fichier, buffer, sizeof(buffer));
   if(lu <= 0) return NULL;
   token = strtok(buffer, "\n");
   //printf("buffer ->\n---->%s\n", buffer);
   /* walk through other tokens */
   while(token != NULL){
      if(nombreImprimante-1 == i){
         char* tokenSansFin = malloc((strlen(token)-1)*sizeof(char));
         strncpy(tokenSansFin, token, strlen(token)-1);
         listeImprimantes[i] = extractImprimante(tokenSansFin);
      }
      else 
         listeImprimantes[i] = extractImprimante(token);
      
      //printf( "--> %s\n", token);
      token = strtok(NULL, "\n");
      i++;
   }
   close(fichier);
   return listeImprimantes;
}

void traiter(mode_t bit, char c1, char c2, int* indice_mode, char* modes){
    modes[(*indice_mode)++] = bit ? c1:c2;
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

char* cheminAbsolu(char* chemin_relatif){
    char* result = malloc(200*sizeof(char));
    chdir(chemin_relatif);
    getcwd(result, 200);     
    return result;
}

void testCheminAbsolu(void){
   struct stat status;
   struct stat status2;

   //controle existance fichier et ouverture tube en lecture
   CU_ASSERT(strcmp(cheminAbsolu("~/.bashrc"),"/home/sultano/.bashrc"));

}

void testDroitsFichier(void){
   FILE* fic = fopen("Objects/new_fic.txt", "w");
   char *args[7];
   args[1] = "-P";
   args[2] = "francino"; 
   args[3] = "Objects/new_fic.txt";
   args[4] = "Objects/tubeClient";
   args[5] = NULL;
   Message* mex = buildMessage('i', args);

   
   FILE* fic2 = fopen("fichierConfiguration", "r");
   char *args2[7];
   args2[1] = "-P";
   args2[2] = "francino"; 
   args2[3] = "fichierConfiguration";
   args2[4] = "Objects/tubeClient";
   args2[5] = NULL;
   Message* mex2 = buildMessage('i', args2);

   CU_ASSERT(0 == possedeDroits(mex2));
   CU_ASSERT(-1 == possedeDroits(mex));

}

void testChargeImprimante(void){
   CU_ASSERT(NULL != chargeImprimantes("fichierConfiguration"));

}

void testStructConversion(void){
   char *args[7];
   args[1] = "-P";
   args[2] = "francino"; 
   args[3] = "temp2.txt";
   args[4] = "tubeClient";
   args[5] = NULL;
   Message* mex = buildMessage('i', args);
   Message* mex2 = bytesToStruct(structToBytes(mex));
   CU_ASSERT(mex->id_impression == mex2->id_impression);
   CU_ASSERT(mex->lng == mex2->lng);
   CU_ASSERT(mex->type == mex2->type);
   CU_ASSERT(mex->uid == mex2->uid);
   CU_ASSERT(mex->gid == mex2->gid);
   CU_ASSERT(strcmp(mex->imprimante,mex2->imprimante) == 0);
   CU_ASSERT(strcmp(mex->ref_abs,mex2->ref_abs) == 0);
   CU_ASSERT(strcmp(mex->tube_client,mex2->tube_client)==0);
}

void testNombreImprimantes(void){
   CU_ASSERT(3 == nombreImprimantes("fichierConfiguration"));
}

void testSend(void){
   int desc = open("Objects/temp2.txt", O_CREAT|O_WRONLY);
   if(desc == -1)
      perror("error: ");
   char *args[7];
   args[1] = "-P";
   args[2] = "francino"; 
   args[3] = "Objects/temp2.txt";
   args[4] = "Objects/tubeClient";
   args[5] = NULL;
   Message* mex = buildMessage('i', args);   
   CU_ASSERT(send(mex, desc) == 0);
}

void testChargeInfoFile(void){
   int desc = open("Objects/temp.txt", O_RDONLY);
   char *args[7];
   args[1] = "-P";
   args[2] = "francino"; 
   args[3] = "Objects/temp.txt";
   args[4] = "Objects/tubeClient";
   args[5] = NULL;
   Message* mex = buildMessage('i', args);   
   CU_ASSERT(chargeInfoFile(mex)->size == 63);

}

void testExtractImprimante(void){
   char* tmp = "francino Objects/tubeFrancino";
   Imprimante* impr = NULL;
   if((impr = extractImprimante(tmp)) != NULL){
      CU_ASSERT(strcmp(impr->nom,"francino") == 0);
      CU_ASSERT(strcmp(impr->tube,"Objects/tubeFrancino") == 0);
   }   
   impr = extractImprimante("");
   CU_ASSERT(impr == NULL);
}

void testEnvoieFichier(void){
   //construction message
   char *args[7];
   args[1] = "-P";
   args[2] = "francino"; 
   args[3] = "fichierConfiguration";
   args[4] = "Objects/tubeClient";
   args[5] = NULL;
   Message* mex = buildMessage('i', args);
   int desc = open("Objects/temp.txt", O_WRONLY);

   CU_ASSERT(0 == envoieFichier(mex,desc));
}


/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test of testCheminAbsolu()", testCheminAbsolu)) ||
       (NULL == CU_add_test(pSuite, "test of testDroitsFichier()", testDroitsFichier)) ||
       (NULL == CU_add_test(pSuite, "test of testChargeImprimante()", testChargeImprimante)) ||
       (NULL == CU_add_test(pSuite, "test of nombreImprimantes()", testNombreImprimantes)) ||
       //(NULL == CU_add_test(pSuite, "test of StructConversion()", testStructConversion)) ||
       (NULL == CU_add_test(pSuite, "test of send()", testSend)) ||
       (NULL == CU_add_test(pSuite, "test of envoieFichier()", testEnvoieFichier)) ||
       (NULL == CU_add_test(pSuite, "test of extractImprimante()", testExtractImprimante)) ||
       (NULL == CU_add_test(pSuite, "test of testChargeInfoFile()", testChargeInfoFile)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
