#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>



#define SERV_PORT 9527

using namespace std;

int main(int argc, char *argv[]){

    int cfd = 0;
    char buf[BUFSIZ];

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1){
        perror("socket error");
        exit(1);
    }
    // connect 可以体现三次握手
    int ret =  connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret != 0){
        perror("connect error");
        exit(1);
    }

    int connect_cnt = 10;
    while(-- connect_cnt){
        write(cfd,"hello\n",7);
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
    }

    close(cfd);

    return 0;
}