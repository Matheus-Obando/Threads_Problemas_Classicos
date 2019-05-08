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
int buffer[12];//Buffer compartilhado entre produtores e consumidores

void listagem(){ //lista os elementos do buffer
    if(quant == VAZIO){
        printf("Buffer Vazio");
    }
    else{
        for(int i = 0; i < quant; i++){
            printf("%d | ", buffer[i]);
        }
    }
    printf("\n");
}

void insere_item(int id){
    buffer[quant] = id;//o buffer recebe a id do produtor
    quant = quant + 1;
    listagem();
}

void consome_item(int id){
    buffer[quant - 1] = 0;//atribui zero ao espaço do último elemento
    quant = quant - 1;
    listagem();
}

void *produz(void *id){
    for(int i = 0; i < 30; i++){
        sem_wait(&mutex);
        sem_wait(&vazio);
        int *num = id;
        sleep(1);
        insere_item(*num);
        sem_post(&cheio);
        sem_post(&mutex);
        sleep(1);
    }
}
void *consome(void *id){
    for(int i = 0; i < 30; i++){
        sleep(1);
        sem_wait(&mutex);
        sem_wait(&cheio);
        int *num = id;
        sleep(1);
        consome_item(*num);
        sem_post(&vazio);
        sem_post(&mutex);
        sleep(1);
    }
}

int main(){

    pthread_t thread_prod;//thread handles dos produtores
    pthread_t thread_cons;//thread handles dos consumidores
    int produtores[3] = {1,2,3};//ids dos produtores
    int consumidores[3] = {1,2,3};//ids dos consumidores
    
    //iniciando os semaphores
    //O semaphore 'vazio' conta o número de espaços disponíveis no buffer
    //O semaphore 'cheio' conta o número de espaços ocupados no buffer

    sem_init(&mutex, 0, 1);//semaphore binário
    sem_init(&vazio, 0, 12);//semaphore contador, o buffer começa vazio
    sem_init(&cheio, 0, 0);//semaphore contador
    
    //criando as threads
    for(int i = 0; i < 3; i++){
        pthread_create(&thread_prod, NULL, produz, &produtores[i]);
        pthread_create(&thread_cons, NULL, consome, &consumidores[i]);
    }

    listagem();

    //inicializando as threads
    pthread_join(thread_prod, NULL);
    pthread_join(thread_cons, NULL);
    
	return 0;
}
