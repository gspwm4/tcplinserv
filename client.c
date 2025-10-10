#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(){
    int sockcl = socket(AF_INET,SOCK_STREAM,0);
    if(sockcl == -1){
        perror("socket failed");
        return -1;
    }
    struct sockaddr_in addr = {0};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    if((inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr)) == -1)
    {
        perror("inet pton failed");
        return -1;
    }
    if(connect(sockcl,(struct sockaddr *) &addr,sizeof(addr)) == -1){
        close(sockcl);
        perror("connect failed");
        return -1;
    }
    while(1){
    char send_buff[255] = {0};
    char recv_buff[255] = {0};

    printf("> ");
    fflush(stdout);

    if(fgets(send_buff,sizeof(send_buff),stdin) == NULL){
        if(feof(stdin)){
            printf("EOF was reached by pressing Ctrl + d\n");
            break;
        }
        else{
            perror("fgets failed");
            break;
        }
    }
    if(strlen(send_buff) == 0){
        continue;
    }

    ssize_t send_mess = send(sockcl,send_buff,strlen(send_buff), 0);
    if(send_mess == -1){
        perror("send failed");
        break;
    }
    ssize_t recv_mess = recv(sockcl,recv_buff,sizeof(recv_buff)-1,0);
    if(recv_mess == -1){
        perror("recv failed");
        break;
    }
    if(recv_mess == 0){
        printf("server disconnected\n");
        break;
    }
    recv_buff[recv_mess] = '\0';
    }
    close(sockcl);
    return 0;
}
