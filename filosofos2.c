//TRABALHO DE SISTEMAS OPERACIONAIS
//Alunos: Matheus Obando, Caio Câmara
//Problema Clássico: O jantar dos filósofos

//Definindo as bibliotecas
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//Definindo os possiveis estados dos filosofos (por questoes de simplificação do código)
#define PENSANDO 0
#define FOME 1
#define COMENDO 2

//Definindo os valores para a movimentação no vetor de filósofos
#define ESQUERDA (id + 4) % 5 //ex: filosofo 5 (indice 4)
                               //Filósofo a esquerda: (4 + 4) % 5 = 3 (índice)
#define DIREITA (id + 1) % 5 //ex: filosofo 5 (indice 4)
                              //Filósofo a esquerda: (4 + 1) % 5 = 0 (índice)

sem_t mutex;//semaforo utilizado na exclusao mutua

//Definindo a estrutura de dados dos filósofos
typedef struct{
    int valor;
    int estado;
    sem_t semaphore;//cada filósofo possui um semaphore
}Filosofo;

Filosofo filosofos[5];//vetor de filosofos global

void verifica(int id){
    if(filosofos[id].estado == FOME && filosofos[ESQUERDA].estado != COMENDO && filosofos[DIREITA].estado != COMENDO){
        //Se o filósofo estiver com fome e os filósfos a sua esquerda e a sua direita
        //não estiverem comendo então:
        filosofos[id].estado = COMENDO;//o filosofo pode comer
        sleep(1);
        printf("Filósofo %d: COMENDO (garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
        sleep(1);
        //Obs: um dos garfos pertence ao filósofo (mesmo índice)        
        sem_post(&filosofos[id].semaphore);
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

    pthread_t thread_id[5];//armazena os handles das threads de cada filosofo

    sem_init(&mutex, 0, 1); //cria o semaphoro pra exclusão mutua

    //inicializando cada struct dos filósofos
    for (int i = 0; i < 5; i++){
        Filosofo f;
        f.valor = i;
        f.estado = PENSANDO;
        sem_init(&f.semaphore, 0, 0);//inicializa o semaphore de cada filósofo
        filosofos[i] = f;
    }

    //Criando as threads
    printf("Estados iniciais:\n\n");
    for (int i = 0; i < 5; i++){
        pthread_create(&thread_id[i], NULL, inicia, &filosofos[i]);
        //No estado inicial todos os filósofos estão pensando
        printf("\tFilósofo %d: PENSANDO\n", i + 1);
    }
    printf("\n");

    //executando as threads
    for (int i = 0; i < 5; i++){
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
