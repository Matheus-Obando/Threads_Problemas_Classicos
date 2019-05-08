//Problema Clássico: O jantar dos filósofos

//Definindo as bibliotecas
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//Definindo os possiveis estados dos filósofos (por questões de simplificação do código)
#define PENSANDO 0
#define FOME 1
#define COMENDO 2

//Definindo os valores para a movimentação no vetor de filósofos
#define ESQUERDA (id + 4) % 5 //ex: filósofo 5 (indice 4)
                               //Filósofo a esquerda: (4 + 4) % 5 = 3 (índice)
#define DIREITA (id + 1) % 5 //ex: filósofo 5 (indice 4)
                              //Filósofo a direita: (4 + 1) % 5 = 0 (índice)

sem_t mutex;//semaphore utilizado na exclusão mútua

//Definindo a estrutura de dados dos filósofos
typedef struct{
    int valor;
    int estado;
    sem_t semaphore;//cada filósofo possui um semaphore
}Filosofo;

Filosofo filosofos[5];//vetor de filósofos global

void verifica(int id){
    if(filosofos[id].estado == FOME && filosofos[ESQUERDA].estado != COMENDO && filosofos[DIREITA].estado != COMENDO){
        //Se o filósofo estiver com fome e os filósofos a sua esquerda e a sua direita
        //não estiverem comendo então:
        filosofos[id].estado = COMENDO;//o filósofo pode comer
        sleep(1);
        printf("Filósofo %d: COMENDO (garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
        //Obs: um dos garfos pertence ao filósofo (mesmo índice)        
        sem_post(&filosofos[id].semaphore);
        sleep(1);
    }

}

void pegar_garfo(int id){
    sem_wait(&mutex);
    filosofos[id].estado = FOME;
    printf("Filósofo %d: COM FOME\n", id + 1);
    verifica(id);//Verifica se pode comer
    sem_post(&mutex);
    sem_wait(&filosofos[id].semaphore);
}

void deixar_garfo(int id){
    sem_wait(&mutex);
    filosofos[id].estado = PENSANDO;//O filósofo deixou de comer 
    printf("Filósofo %d: PENSANDO (deixa os garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
    //Como o filósofo devolveu os garfos, os filósofos ao seu lado
    //irão verificar a possibilidade de comer
    verifica(ESQUERDA);
    verifica(DIREITA);
    sleep(1);
    sem_post(&mutex);

}

void *inicia(void *id){
    for(int i = 0; i < 3; i++){
        Filosofo *filosofo = id;
        sleep(1);
        pegar_garfo(filosofo->valor);
        sleep(1);
        deixar_garfo(filosofo->valor);
    }
}

int main(){

    pthread_t thread_id;//armazena os handles das threads de cada filósofo

    sem_init(&mutex, 0, 1); //inicializa o semaphore pra exclusão mutua

    //inicializando as structs de cada filósofo
    for (int i = 0; i < 5; i++){
        Filosofo f;
        f.valor = i;
        f.estado = PENSANDO;
        sem_init(&f.semaphore, 0, 0);//inicializa os semaphores de cada filósofo
        filosofos[i] = f;
    }

    //Criando as threads
    printf("Estados iniciais:\n\n");
    for (int i = 0; i < 5; i++){
        pthread_create(&thread_id, NULL, inicia, &filosofos[i]);
        //No estado inicial todos os filósofos estão pensando
        printf("\tFilósofo %d: PENSANDO\n", i + 1);
    }
    
    printf("\n");
    pthread_join(thread_id, NULL);
 
    return 0;
}
