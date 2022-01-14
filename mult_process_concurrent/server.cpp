#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "wrap.h"

using namespace std;

#define SRV_PORT 9999

void catch_child(int signum){
    while(waitpid(0, NULL, WNOHANG) > 0);
    return;
}

int main(int argc, char **argv){
    int lfd, cfd;
    pid_t pid;
    struct sockaddr_in srv_addr, client_addr;
    char buf[BUFSIZ];
    int ret;
    socklen_t clt_addr_len;

    memset(&srv_addr, 0, sizeof(srv_addr));   //addr to zero
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SRV_PORT);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    Bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    Listen(lfd, 128);
    clt_addr_len = sizeof(client_addr);

    while(1){
        
        cfd = Accept(lfd, (struct sockaddr *)&client_addr, &clt_addr_len);
        // cout << "there is ok" << endl;
        pid = fork();
        if(pid < 0){
            // cout << "what fuck ??"<<endl;
            perror("fork error");
            exit(1);
        }
        if(pid == 0){
            close(lfd);
            break;
        }else{
            struct sigaction act;
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            ret = sigaction(SIGCHLD, &act, NULL);
            if(ret != 0){
                perror("sigaction error");
                exit(1);
            }
            close(cfd);
        }
    }        

    if(pid == 0){
        while( 1){
            ret = Read(cfd, buf, sizeof(buf));
            // cout << "there is child process" << endl;
            if(ret == 0){
                close(cfd);
                exit(1);
            }
            for(int i = 0; i < ret; ++ i)
                buf[i] = toupper(buf[i]);
            write(cfd, buf, ret);
            write(STDOUT_FILENO, buf, ret);
        }
    }

    return 0;
}