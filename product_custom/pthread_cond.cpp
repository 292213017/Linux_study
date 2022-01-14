#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctime> 

#include <pthread.h>

using namespace std;

typedef struct _node_t {
    int data;
    struct _node_t *next;
}node_t;

node_t* head = NULL;
node_t* head_new = NULL;

//cond pthread 
pthread_cond_t cond;
//mutex pthread
pthread_mutex_t mutex;
pthread_mutex_t mutex_1;

void   Delay(int   time)//time*1000为秒数 
{ 
    clock_t   now   =   clock(); 
    while(   clock()   -   now   <   time   ); 
} 

void *producer(void * arg) {
    while(1){
        // pthread_mutex_lock(&mutex);
        node_t* node = (struct _node_t*)malloc(sizeof(node_t));
        if(NULL == node){
            cout << "ERROR: malloc() failed" << endl;
            break;
        }
        memset(node, 0, sizeof(node_t));
        node->data = rand()% 100 + 1;

        node->next = head;
        head = node;
        cout << "product :"<< head->data << endl;

        // pthread_cond_signal(&cond);
        // pthread_mutex_unlock(&mutex);
        Delay(500000);
        
    }
    pthread_exit(NULL);
}

void *customer(void * arg) {
    node_t * tmp = NULL;
    while(1){
        pthread_mutex_lock(&mutex);
        if(NULL == head){
            //   pthread_cond_wait(&cond, &mutex);
        }else{
            // cout << "middle" << endl;
            tmp = head ;
            head = head->next;
            cout << "middle :" << tmp->data << endl;

            // pthread_mutex_lock(&mutex_1);
            node_t* node = (struct _node_t*)malloc(sizeof(node_t));
            node = tmp;
            node->next = head_new;
            head_new = node;

            // pthread_mutex_unlock(&mutex_1);
            
            
        }
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        // sleep(random() %1 + 1);
    }
    pthread_exit(NULL);
}

void *customer2(void * arg) {
    node_t * tmp = NULL;
    while(1){
        pthread_mutex_lock(&mutex);
        if(NULL == head_new){
              pthread_cond_wait(&cond, &mutex);
        }else{
            tmp = head_new ;
            head_new = head_new->next;
            cout << "customer :" << tmp->data << endl;
            free(tmp);
        }
        // cout << "222" << endl;
        pthread_mutex_unlock(&mutex);
        // sleep(random() %1 + 1);
        
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid0 = -1,tid1 = -1 , tid2 = - 1;

    // init
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_1, NULL);
    pthread_cond_init(&cond, NULL);

    srandom(getpid());
    pthread_create(&tid0, NULL, producer, NULL);
    pthread_create(&tid1, NULL, customer, NULL);
    pthread_create(&tid2, NULL, customer2, NULL);

    pthread_join(tid0, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_1);
    pthread_cond_destroy(&cond);

    return 0;
}