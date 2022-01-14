#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "wrap.h"

using namespace std;

#define SRV_PORT 9999

void* do_work(void * arg){
    int n;
    char buf[BUFSIZ];
    int  cfd = *(int *) arg;
    while(1){
        n = Read(cfd, buf, sizeof(buf));
        if(n == 0){
            //n 为 0  的时候，一定表示对端关闭 -1 才是表示出错  >0表示的是读到的字节数
            perror("read over");
            break;
        }
        for(int i = 0; i < n; i++){
            buf[i] = toupper(buf[i]);
        }
        Write(cfd, buf, n);
        Write(STDOUT_FILENO, buf, n);
    }
    return (void*)0;
}

int main(int argc, char **argv){

    int lfd,cfd;
    pthread_t tid;

    struct sockaddr_in srv_addr, clt_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SRV_PORT);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    Bind(lfd, (struct sockaddr*)&srv_addr,sizeof(srv_addr));

    Listen(lfd, 128);

    while(1){
        socklen_t clt_addr_len = sizeof(clt_addr);
        cfd = Accept(lfd, (struct sockaddr*) &clt_addr, &clt_addr_len);
        pthread_create(&tid, NULL, do_work, (void*)&cfd);
        pthread_detach(tid);
    }

    return 0;
}