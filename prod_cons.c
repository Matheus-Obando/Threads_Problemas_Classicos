//TRABALHO DE SISTEMAS OPERACIONAIS
//Alunos: Matheus Obando, Caio Câmara
//Problema Clássico: Problema do produtor e do consumidor

//Definindo as bibliotecas
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//definindo os semaphores
sem_t vazio, cheio, mutex;

int produtor[3] = {0, 1, 2};//ids
int consumidor[3] = {0, 1, 2};//ids

#define VAZIO 0
#define CHEIO 4 //valor máximo do buffer dado como exemplo
int quant = 0; //Quantidade inicial inicia como vazio

void listagem(){

    if(quant == VAZIO){
        printf("%d | ",quant);
    }
    else{
        for(int i = 0; i < quant; i++){
            printf("%d | ", i+1);
        }
    }
    printf("\n");
}


void insere_item(int id){
    if(quant < CHEIO){
        quant = quant + 1;
        //printf("Quantidade atual: %d (inseriu +1)\n",quant);
        listagem();
        sleep(1);
    }
}

void consome_item(int id){
    if(quant > VAZIO){
        quant = quant - 1;
        //printf("Quantidade atual: %d (consumiu -1)\n",quant);
        listagem();
        sleep(1);
    }
}

void *produz(void *prod){
    for(int i = 0; i < 30; i++){
        sem_wait(&vazio);
        sem_wait(&mutex);
        int *id = prod;
        //printf("iteração produtor %d: %d\n", *id, i);
        insere_item(*id);
        sem_post(&mutex);
        sem_post(&cheio);
    }
}
void *consome(void *cons){
    for(int i = 0; i < 30; i++){
        sem_wait(&cheio);
        sem_wait(&mutex);
        int *id = cons;
        //printf("iteração consumidor %d: %d\n", *id, i);
        consome_item(*id);
        sem_post(&mutex);
        sem_post(&vazio);
    }
}

int main(){

    pthread_t thread_prod[3];//thread handle do produtor
    pthread_t thread_cons[3];
    
    //iniciando os semaphores
    sem_init(&mutex, 0, 1);//semaphore binário
    sem_init(&vazio, 0, 4);//semaphore contador, o buffer começa vazio
    sem_init(&cheio, 0, 0);//semaphore contador

    //for(int i = 0; i < 3; i++){
    //    sem_init(&produtor[i], 0, 0);
    //    sem_init(&consumidor[i], 0, 0);
    //}
    
    for(int i = 0; i < 3; i++){
        pthread_create(&thread_prod[i], NULL, produz, &produtor[i]);
        pthread_create(&thread_cons[i], NULL, consome, &consumidor[i]);
    }

    for(int i = 0; i < 3; i++){
        pthread_join(thread_prod[i], NULL);
        pthread_join(thread_cons[i], NULL);
    }

	return 0;
}
