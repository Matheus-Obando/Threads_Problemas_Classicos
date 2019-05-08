//Problema clássico: Leitor x Escritor

#include<stdio.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>

sem_t wmutex;
sem_t rmutex;
char buf[20];
int nr=0;

void initwrite(){//Inicializa o buffer antes da chamada das threads
    printf("Escreva: ");
    scanf("%s", &buf);
}

void startread(){
    sem_wait(&rmutex);
    nr = nr+1;
    if(nr == 1){
        sem_wait(&wmutex);
    }
    sem_post(&rmutex);
}

void endread(){
    sem_wait(&rmutex);   
    nr = nr-1;
    if(nr == 0){
        sem_post(&wmutex);
    }
    sem_post(&rmutex);
    sleep(1);
}

void startwrite(){
    sem_wait(&wmutex);
}

void endwrite(){
    sem_post(&wmutex);
}

void *reader(){
    while(1){
        startread();
        printf("Lendo conteúdo: - %s \n", buf);
        sleep(1);
        endread();
    }
}

void *writer(){
    while(1){
        sleep(1);
        startwrite();
        printf("Escreva: ");
        scanf("%s", &buf);
        endwrite();
    }
}

int main(){
    int i;

    sem_init(&wmutex,0,1);
    sem_init(&rmutex,0,1);
    initwrite();
    pthread_t thread_r;
    pthread_t thread_w;

    for(i=0; i<4; i++){
        pthread_create(&thread_r, NULL, reader, NULL);
    }

    for(i=0; i<2; i++){
        pthread_create(&thread_w, NULL, writer, NULL);
    }

    pthread_join(thread_r,NULL);
    pthread_join(thread_w,NULL);

    return 0;

}
