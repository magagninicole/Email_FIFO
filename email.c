#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include "fila.h"

sem_t empty; //Semaforo vazio
sem_t full; //Semaforo cheio
int counter; //Contador
pthread_mutex_t mutex; //Mutex serve para dar acesso a apenas um processo por vez
pthread_attr_t attr; //
int posicao = 0; //Posição no banco de dados
#define NUMBER_OF_STRING 5 //Número de e-mails por vez
#define MAX_STRING_SIZE 40 //Tamanho máximo dos e-mails
#define MAX_POSI 100 //Tamanho máximo do banco de dados
#define RAND_DIVISOR 100000000

char database [MAX_POSI][MAX_STRING_SIZE];
FILA inbox;

/* Add an item to the buffer */
int insert_item(char item[])
{
    /*Quando a caixa de inbox não está vazia*/
    if (counter < NUMBER_OF_STRING) {
        entrada(&inbox,item); //Adiciona até estar
        counter++;
        return 0;
    } else { //Se está cheia, retorna -1
        return -1;
    }
}


int remove_item(char item[])
{
    /*Se o inbox não estiver vazio, remove o item e decrementa
    o contador */
    if (counter > 0) {
        char *le = leitura(&inbox);
        for(int i =0; i<MAX_STRING_SIZE;i++){
            database[posicao][i] = le[i];
            item[i] = le[i];
        }
        posicao++;
        counter--;
        return 0;
    } else { //Se o inbox estiver vazio
        return -1;
    }
}

void *envia(void *pno)
{
    char item[MAX_STRING_SIZE];

    while (counter < NUMBER_OF_STRING) {
        /* sleep for a random period of time */
         int rNum = rand() / RAND_DIVISOR;
        sleep(rNum);

        /* Gera e-mail */
         for(int i=0; i<MAX_STRING_SIZE;i++){
              char randletra = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random () % 26];
             item[i] = randletra;
         }
     
        sem_wait(&empty); //Espera o inbox estar vazio
        pthread_mutex_lock(&mutex); //Bloqueia o objeto
        if(counter < NUMBER_OF_STRING){
            if (insert_item(item)) {//Insere o item
                printf("%d Remetente relatou um erro de envio\n", *(int *)pno);
            } else {
                printf("%d E-mail enviado %s\n", *(int *)pno, item);
            }
        }

        /*printf("FILA DE E-MAILS: \n\n");
           imprime(&inbox);*/
        
        pthread_mutex_unlock(&mutex);
        sem_post(&full); 
    }
    return NULL;
}

void *recebe(void *cno)
{
    char item[MAX_STRING_SIZE];

    while (counter < NUMBER_OF_STRING) {
        /* sleep for a random period of time */
        int rNum = rand() / RAND_DIVISOR;
        sleep(rNum);

        /*Pega o semaforo cheio*/
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        if(counter > 0){
        if (remove_item(item)) {
           printf("%d Leitor relatou um erro de leitura \n", *(int *)cno);
        } else {
            printf("%d E-mail recebido e lido %s\n", *(int *)cno, item);
        }
    }
        /*printf("FILA DE E-MAILS: \n\n");
            if(counter == 0){
                printf("\nTodos e-mails foram lidos\n\n");
            }else{
                imprime(&inbox);     
            }*/
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(void)
{
   
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);

    sem_init(&full, 0, 0); //Inicializa full em 0
    sem_init(&empty, 0, NUMBER_OF_STRING); //inicializa empty no tamanho do buffer

    pthread_attr_init(&attr); //Inicializa a thread
    counter = 0;

    int ids[5] = { 1, 2, 3, 4, 5};
    pthread_t remetente[5]; 
    pthread_t leitor[5]; 

    for (int i = 0; i < 5; i++) {
        pthread_create(
            &remetente[i], NULL, (void *)envia, (void *)&ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_create(
            &leitor[i], NULL, (void *)recebe, (void *)&ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(remetente[i], NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(leitor[i], NULL);
    }

   /* printf("CAIXA DE E-MAILS:");
    for(int i=0; i<posicao;i++){
        printf("E-MAIL %d",i);
        for(int j=0; j<MAX_STRING_SIZE;i++){
            printf("%c", database[i][j]);
        }
    }*/

    return EXIT_SUCCESS;
}