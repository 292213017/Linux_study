#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>



#define SERV_PORT 9527

using namespace std;

int main(int argc, char *argv[]){

    int lfd = 0 , cfd = 0;
    int ret = 0;
    char buf[BUFSIZ], client_IP[1024];

    struct sockaddr_in serv_addr, client_addr;

    socklen_t client_addr_len, client_IP_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket error");
        exit(1);
    }

    int ret_bind = bind(lfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret_bind == -1){
        perror("bind error");
        exit(1);
    }

    listen(lfd,128);

    client_addr_len = sizeof(client_addr);
    // accept 可以体现三次握手
    cfd = accept(lfd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(cfd == -1){
        perror("accept error");
        exit(1);
    }

    cout << "client ip is " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)) << "   port is " << ntohs(client_addr.sin_port) << endl;

    while(1){
        ret = read(cfd,buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);

        for(int i = 0; i < ret; i++){
            buf[i] = toupper(buf[i]);
        }

        write(cfd,buf,ret);
    }
    close(lfd);
    close(cfd);
    return 0;
}