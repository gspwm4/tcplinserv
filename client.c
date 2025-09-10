#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int sock_cl = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int inet_convert = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if(inet_convert == -1){
        fprintf(stderr, "inet error\n");
        return -1;
    }
    int conn_socket = connect(sock_cl, (struct sockaddr *) &addr, sizeof(addr));
    if(conn_socket == -1){
        fprintf(stderr, "connect error");
        return -1;
    }
    write(sock_cl, "Hello\n", 6);
    char buff[128];
    ssize_t read_buff = read(sock_cl, buff, 128);
    if(read_buff == -1){
        fprintf(stderr, "read failed\n");
        return -1;
    }
    if(read_buff == 0){
        fprintf(stdout, "read success\n");
    }
    write(STDOUT_FILENO, buff, read_buff);
    close(sock_cl);
    return 0;
}
