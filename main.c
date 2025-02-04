/*Implement a concurrent program with n threads that sorts an input file. The program must proceed in parallel on different files, according to the following specifications.

The program (which name pgrm) receives 3 parameters from command line

pgrm n strA strB
where:

n is an integer value
strA is a string that identifies the name of n input files of name 'strA1.txt, strA2.txt, …, strAn.txt'
strB is a string that identifies the name of n output files of name 'strB1.txt, strB2.txt, …, strBn.txt'*/ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    char name[20];
} filename;

//Prototipi del caso
void *readT(void* arg); 


void *readT(void* arg){
    //Cast del puntatore
    char *filename = (char*) arg;
    //printf("Ciao sono il thread con file %s\n", filename);
    FILE *fp = fopen(filename, "r");
    if (fp==NULL)
    {
        perror("Errore nell'apertura del file\n");
        exit(-1);
    }

    int n, temp, i = 0;
    //Prima riga del file:
    fscanf(fp, "%d", &n);
    //La prima riga è sempre il totale delle entries
    //Beccatevi sta allocazione dinamica
    int* vect = (int*) malloc(n*sizeof(int));
    while(fscanf(fp, "%d", &temp)!=EOF&&i<n){
        vect[i] = temp;
        i++;
    }

    for(int j = 0; j<n; j++){
        printf("%d\n", vect[i]);
    }

    free(vect);
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
        char temp[10];
        char *prefix = "./";
        //Converto numero i in lettera

        //Ora temp contiene la parte finale del nome del file: A.txt
        sprintf(temp, "%d.txt", i+1); 
        sprintf(fileVect[i].name, "%s%s%s", prefix, argv[2], temp);

    }
  
    for(int i = 0; i<n; i++){
        
        //Creo il thread e a ciascuno passo il proprio pt, funzione e argomenti
        rc = pthread_create(&pt[i], NULL, readT, (void*) &fileVect[i].name);

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