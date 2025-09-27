#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(){
    int sock_cl = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_cl == -1){
        perror("socket failed");
        return -1;
    }
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if((inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)) == -1)
    {
        perror("inet pton failed");
        return -1;
    }
    int conn_socket = connect(sock_cl, (struct sockaddr *) &addr, sizeof(addr));
    if(conn_socket == -1){
        perror("connect failed");
        return -1;
    }
    while(1){
    char send_buff[256] = {0};
    char recv_buff[256] = {0};

    if(fgets(send_buff, sizeof(send_buff), stdin) == NULL)
    {
        perror("fgets failed");
        return -1;
    }
    if(strncmp(send_buff, "exit", 4) == 0){
        printf("exit\n");
        break;
    }
    ssize_t write_buff = write(sock_cl, send_buff, strlen(send_buff));
    if(write_buff == -1){
        perror("write failed");
        return -1;
    }
    ssize_t read_buff = read(sock_cl, recv_buff, sizeof(recv_buff)-1);
    if(read_buff == -1){
        perror("read failed");
        return -1;
    }
    recv_buff[read_buff] = '\0';
    }
    close(sock_cl);
    return 0;
}
