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

typedef struct {
    filename inFile;
    filename outFile;
} argThread;

//Prototipi del caso
void *readT(void* arg); 
void bubbleSort(int * arg, int n);

void bubbleSort(int* arg, int n){
    int *vect = arg;
    int lenght = n;
    int i, j, l = 0, r = n-1, flag = 1, temp;
    for(i = 0; i < n&&flag; i++){
        flag = 0;
        for(j = l; j<r-i+l; j++){
            if(vect[j]>vect[j+1]){
                flag = 1;
                temp = vect[j];
                vect[j] = vect[j+1];
                vect[j+1] = temp;
            }
        }
    }
    return NULL;
}

void *readT(void* arg){
    //Cast del puntatore
    argThread *filename = (argThread*) arg;
    printf("Ciao sono il thread con file %s\n", filename->inFile.name);
    FILE *fp = fopen(filename->inFile.name, "r");
    FILE *fw = fopen(filename->outFile.name, "w");

    if (fp==NULL||fw==NULL)
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
        printf("Dal file %s: %d\n", filename->inFile.name, vect[j]);
    }
    
    //Fino a qui ho: vect che contiene il contenuto di ogni file, ORDINO:
    bubbleSort(vect, n);
    for(int j = 0; j<n; j++){
        printf("ORDINATI: file %s: %d\n", filename->outFile.name, vect[j]);
    }

    //Ora stampo anche nel file:
    fprintf(fw, "%d\n", n);
    for(int j = 0; j<n; j++){
        fprintf(fw,"%d\n", vect[j]);
    }

    fclose(fp);
    free(vect);
    return NULL;
}

int main(int argc, char* argv[]){

    setbuf(stdout, NULL);
    if(argc!=4){
        perror("Correct format: <prog> <num> <strIn> <strOut>\n");
        exit(-1);
        }

    int n = atoi(argv[1]), rc;
    pthread_t pt[n];    

    //Vettore che tiene la mia struttura con dentro il nome del file
    argThread fileVect[n];
    
    //Creo un vettore con i nomi del file di Input:
    for(int i = 0; i<n; i++){
        char temp[10];
        char *prefix = "./";
        //Ora temp contiene la parte finale del nome del file: A.txt
        sprintf(temp, "%d.txt", i+1); 
        sprintf(&fileVect[i].inFile, "%s%s%s", prefix, argv[2], temp);
    }
    //Creo un vettore con i nomi del file di Output:
    for(int i = 0; i<n; i++){
        char temp[10];
        char *prefix = "./";
        //Ora temp contiene la parte finale del nome del file: A.txt
        sprintf(temp, "%d.txt", i+1); 
        sprintf(&fileVect[i].outFile, "%s%s%s", prefix, argv[3], temp);
    }
  
    for(int i = 0; i<n; i++){
        
        //Creo il thread e a ciascuno passo il proprio pt, funzione e argomenti
        rc = pthread_create(&pt[i], NULL, readT, (void*) &fileVect[i]);

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