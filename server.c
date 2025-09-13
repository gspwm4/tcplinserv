#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        close(sockfd);
        perror("socket failed");
        return -1;
    }
    else{
        printf("socket created...\n");
    }
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bind_stat = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    char ip_addr[INET_ADDRSTRLEN];
    if((inet_ntop(AF_INET, &(addr.sin_addr), ip_addr, INET_ADDRSTRLEN) == NULL)){
        perror("inet ntop failed");
        close(sockfd);
        return -1;
    }

    if(bind_stat == -1){
        perror("bind failed");
        return -1;
    }
    else{
        printf("bind addr %s on port %d successful...\n", ip_addr, ntohs(addr.sin_port));
    }
    while(1){
    if(listen(sockfd, 5) == -1){
        perror("listen failed");
        continue;
    }
    else{
        printf("listen successful...\n");
    }
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size = sizeof(peer_addr);
    int acceptfd = accept(sockfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
    if(acceptfd == -1){
        close(acceptfd);
        perror("accept failed");
        continue;
    }
    ssize_t read_eof;
    char buff[128];
    read_eof = read(acceptfd, buff, 128);
    if(read_eof == -1){
        close(acceptfd);
        close(sockfd);
        fprintf(stderr, "read failed\n");
        continue;
    }
    if(read_eof == 0){
        fprintf(stdout, "EOF success\n");
    }
    write(STDOUT_FILENO, buff, read_eof);
    write(acceptfd, buff, read_eof);
    fprintf(stdout, "read: %zu\n", read_eof);

    close(acceptfd);
    }
    close(sockfd);

    return 0;
}
