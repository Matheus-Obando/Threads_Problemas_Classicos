//Prova de Sistemas Operacionais

//T1 e T2 produzem item nos buffers 1 e 2 respectivos
//T3 consome os elementos dos buffers 1 e 2 e os usa pra produzir um item
//no buffer 3
//T4 consome os itens do buffer 3
//Pode-se abstrair os acessos ao buffer com mensagens

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t prodex, conex, final;
//sem_t mutex;

int count = 0;
int cond = 0;

void produz(int id){
    printf("T%d insere item no buffer %d\n",id,id);
    sleep(1);
}

void consome(int id){
    if(id == 3){
        for(int i = 0; i < 2; i++){
            count++;
            printf("T%d consome item do buffer %d\n",id,count);
            sleep(1);
            if(count == 2){
                count = 0;
                produz(id);
                sem_post(&final);
            }
        }
    }
    else{
        printf("T4 consome item do buffer 3\n\n");
        sleep(1);
    }
}

void *T1T2(void *id){
    while(1){
        sem_wait(&prodex);
        //sem_wait(&mutex);
        int *num = id;
        produz(*num);
        sleep(1);
        //sem_post(&mutex);
        cond++;
        if(cond == 2){
            sem_post(&conex);
            cond = 0;
        }
    }
}
void *T3(void *id){
    while(1){
        sem_wait(&conex);
        //sem_wait(&mutex);
        int *num = id;
        consome(*num);
        sleep(1);
        //sem_post(&mutex);
        sem_post(&prodex);
    }
}

void *T4(void *id){
    while(1){
        sem_wait(&final);
        //sem_wait(&mutex);
        int *num = id;
        consome(*num);
        sleep(1);
        //sem_post(&mutex);
        sem_post(&prodex);
    }
}

int main(){

    pthread_t T[4];
    int id[4]= {1,2,3,4};
    
    //sem_init(&mutex, 0, 1);
    sem_init(&prodex, 0, 2);
    sem_init(&conex, 0, 0);
    sem_init(&final, 0, 0);
    
    for(int i = 0; i < 2; i++){
        pthread_create(&T[i], NULL, T1T2, &id[i]);
    }

    pthread_create(&T[2], NULL, T3, &id[2]);
    pthread_create(&T[3], NULL, T4, &id[3]);

    for(int i = 0; i < 4; i++){
        pthread_join(T[i], NULL);
    }

	return 0;
}
