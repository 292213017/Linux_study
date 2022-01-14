#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

using namespace std;

typedef struct _node_t {
    int data;
    struct _node_t *next;
}node_t;

node_t* head = NULL;

void *producer(void * arg) {
    while(1){
        node_t* new_head = (struct _node_t*)malloc(sizeof(node_t));
        if(NULL == new_head){
            cout << "ERROR: malloc() failed" << endl;
            break;
        }
        memset(new_head, 0, sizeof(node_t));
        new_head->data = rand()% 100 + 1;

        new_head->next = head;
        head = new_head;

        cout << "product :" << head->data << endl;

        sleep(random() % 3 + 1);
    }
    pthread_exit(NULL);
}

void *customer(void * arg) {
    node_t * tmp = NULL;
    while(1){
        if(NULL == head){

        }else{
            tmp = head ;
            head = head->next;
            cout << "customer " << tmp->data << endl;
            sleep(random() %3 + 1);
            free(tmp);
        }

    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid1 = -1 , tid2 = - 1;

    srandom(getpid());
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, customer, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}