//TRABALHO DE SISTEMAS OPERACIONAIS
//Alunos: Matheus Obando, Caio Câmara
//Problema Clássico: O jantar dos filósofos

//Definindo as bibliotecas
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//Definindo os possíveis estados dos filósofos (por questões de simplificação do código)
#define PENSANDO 0
#define FOME 1
#define COMENDO 2

//Definindo os valores para a movimentação no vetor de filósofos
#define ESQUERDA (id + 4) % 5 //ex: filósofo 5 (índice 4)
                               //Filósofo a esquerda: (4 + 4) % 5 = 3 (índice)
#define DIREITA (id + 1) % 5 //ex: filósofo 5 (índice 4)
                              //Filósofo a direita: (4 + 1) % 5 = 0 (índice)

//Dados globais
int estados[5];//Armazena os estados dos filósofos
int filosofos[5] = {0,1,2,3,4};//ids dos filósofos

sem_t mutex;//semaphore utilizado na exclusão mútua
sem_t semaphores[5];//armazena os semaphores de cada filósofo
 
void verifica(int id){
    if(estados[id] == FOME && estados[ESQUERDA] != COMENDO && estados[DIREITA] != COMENDO){
        //Se o filósofo estiver com fome e os filósofos a sua esquerda e a sua direita
        //não estiverem comendo então:
        estados[id] = COMENDO;//o filósofo pode comer
        sleep(1);
        printf("Filósofo %d: COMENDO (garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
        //Obs: um dos garfos pertence ao filósofo (mesmo índice)
        sleep(1);        
        sem_post(&semaphores[id]);
    }

}

void pegar_garfo(int id){
    sem_wait(&mutex);
    estados[id] = FOME;
    printf("Filósofo %d: COM FOME\n", id + 1);
    verifica(id);//Verifica se pode comer
    sem_post(&mutex);
    sem_wait(&semaphores[id]);
}
void deixar_garfo(int id){
    sem_wait(&mutex);
    estados[id] = PENSANDO;//O filósofo deixou de comer 
    printf("Filósofo %d: PENSANDO (deixa os garfos %d e %d)\n", id + 1, id + 1, DIREITA + 1);
    //Como o filósofo devolveu os garfos, os filósofos ao seu lado
    //irão verificar a possibilidade de comer
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

    sem_init(&mutex, 0, 1); //inicializa o semaphore pra exclusão mútua

    //inicializando os semaphores de cada filósofo:
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
