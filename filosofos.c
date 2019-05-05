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

//Dados globais
int estados[5];//Armazena os estados dos filósofos
int filosofos[5] = {0,1,2,3,4};//ids dos filósofos

sem_t mutex;//semaforo utilizado na exclusao mutua
sem_t semaphores[5];//armazena os semaphoros de cada filosofo
 
void verifica(int id){
    if(estados[id] == FOME && estados[ESQUERDA] != COMENDO && estados[DIREITA] != COMENDO){
        //Se o filósofo estiver com fome e os filósfos a sua esquerda e a sua direita
        //não estiverem comendo então:
        estados[id] = COMENDO;//o filosofo pode comer
        printf("Filósofo %d: COMENDO (garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
        //Obs1: um dos garfos pertence ao filósofo (mesmo índice)
        sleep(1);        
        sem_post(&semaphores[id]);
    }

}

void pegar_garfo(int id){
    sem_wait(&mutex);
    estados[id] = FOME;
    printf("Filósofo %d: COM FOME\n", id + 1);
    sleep(1);
    verifica(id);//Verifica se pode comer
    sem_post(&mutex);
    sem_wait(&semaphores[id]);
}
void deixar_garfo(int id){
    sem_wait(&mutex);
    
    estados[id] = PENSANDO;//O filósofo deixou de comer
    //printf("Filósofo %d deixa os garfos %d e %d\n", id + 1, ESQUERDA + 1, DIREITA + 1); 
    printf("Filósofo %d: PENSANDO (deixa os garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
    //Como o filósofo devolveu os garfos, os filósofos ao seu lado
    //Irão verificar a possibilidade de comer
    verifica(ESQUERDA);
    verifica(DIREITA);
    sem_post(&mutex);

}

void *inicia(void *id){
    for(int i = 0; i < 2; i++){
        int *filosofo = id;
        sleep(1);
        pegar_garfo(*filosofo);
        sleep(1);//tempo de espera entre pegar e deixar o garfo
        deixar_garfo(*filosofo);
    }
}

int main(){
    pthread_t thread_id[5];//armazena os handles das threads de cada filosofo

    sem_init(&mutex, 0, 1); //cria o semaphoro pra exclusão mutua

    //inicializando os semaphoros de cada filosofo:
    for (int i = 0; i < 5; i++){
        sem_init(&semaphores[i], 0, 0);
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
