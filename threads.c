#include<stdio.h>
#include<pthread.h>
#include<time.h>

void *Thread0(){
    int i = 0;
    for(i = 1; i < 10; i++){
        printf("Thread0 - %d\n",i);
        sleep(3);
    }
}

void *Thread1(){
    int i = 0;
    for(i = 10; i < 20; i++){
        printf("Thread1 - %d\n",i);
        sleep(1);
    }
}

int main(){

    pthread_t t0, t1;//thread handles
    int ret0, ret1;

    //criando threads
    ret0 = pthread_create(&t0, NULL, Thread0, NULL);
    ret1 = pthread_create(&t1, NULL, Thread1, NULL);

    //executando threads
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("Main acabou\n");

    return 0;
}