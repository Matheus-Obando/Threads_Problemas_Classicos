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

#define VAZIO 0 
#define CHEIO 12 //Tamanho máximo do buffer dado como exemplo
int quant = 0; //Quantidade inicial inicia como vazio

void listagem(){ //Por questões de simplificação, será listado apenas a quantidade
                 //de elementos
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

void insere_item(){
    quant = quant + 1;
    listagem();
}

void consome_item(){
    quant = quant - 1;
    listagem();
}

void *produz(){
    for(int i = 0; i < 30; i++){
        sem_wait(&vazio);
        sem_wait(&mutex);
        sleep(1);
        insere_item();
        sem_post(&mutex);
        sem_post(&cheio);
    }
}
void *consome(void *cons){
    for(int i = 0; i < 30; i++){
        sem_wait(&cheio);
        sem_wait(&mutex);
        sleep(1);
        consome_item();
        sem_post(&mutex);
        sem_post(&vazio);
    }
}

int main(){

    pthread_t thread_prod[3];//thread handles dos produtores
    pthread_t thread_cons[3];//thread handles dos consumidores
    
    //iniciando os semaphores
    //O semaphore 'vazio' conta o número de espaços disponíveis no buffer
    //O semaphore 'cheio' conta o número de espaços ocupados no buffer

    sem_init(&mutex, 0, 1);//semaphore binário
    sem_init(&vazio, 0, 12);//semaphore contador, o buffer começa vazio
    sem_init(&cheio, 0, 0);//semaphore contador
    
    //criando as threads
    for(int i = 0; i < 3; i++){
        pthread_create(&thread_prod[i], NULL, produz, NULL);
        pthread_create(&thread_cons[i], NULL, consome, NULL);
    }

    listagem();

    //inicializando as threads
    for(int i = 0; i < 3; i++){
        pthread_join(thread_prod[i], NULL);
        pthread_join(thread_cons[i], NULL);
    }

	return 0;
}
