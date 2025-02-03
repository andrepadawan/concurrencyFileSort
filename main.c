/*Implement a concurrent program with n threads that sorts an input file. The program must proceed in parallel on different files, according to the following specifications.

The program (which name pgrm) receives 3 parameters from command line

pgrm n strA strB
where:

n is an integer value
strA is a string that identifies the name of n input files of name 'strA1.txt, strA2.txt, …, strAn.txt'
strB is a string that identifies the name of n output files of name 'strB1.txt, strB2.txt, …, strBn.txt'*/ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    char name[20];
} filename;

void *read(void* arg); 

void *read(void* arg){
    char *filename = (char*) arg;
    printf("Ciao sono il thread con file %s\n", filename);
    return NULL;
}

int main(int argc, char* argv[]){

    if(argc!=4){
        perror("Correct format: <prog> <num> <strIn> <strOut>\n");
        exit(-1);
        }
    int n = atoi(argv[1]), rc;
    pthread_t pt[n];    

    //Vettore che tiene la mia struttura con dentro il nome del file
    filename fileVect[n];
    
    //Creo un vettore con i nomi del file:
    for(int i = 0; i<n; i++){
        char temp[10], c;
        //Converto numero i in lettera
        c = 'A' + i;
        //Ora temp contiene la parte finale del nome del file: A.txt
        sprintf(temp, "%c.txt", c); 

        //Concateno prima e seconda parte, ma prima copio in filename
        strcpy(fileVect[i].name, argv[2]);
        strcat(fileVect[i].name, temp);
        //printf("%s\n", fileVect[i].name);

    }
  
    for(int i = 0; i<n; i++){
        
        //Creo il thread e a ciascuno passo il proprio pt, funzione e argomenti
        rc = pthread_create(&pt[i], NULL, read, (void*) &fileVect[i].name);

        //Controllo orrori
        if(rc!=0){
            perror("Errore nella creazione dei thread");
            exit(-1);
           }
    }
    for (int i = 0; i < n; i++) {
    pthread_join(pt[i], NULL);
    }
    
    return 0;
}