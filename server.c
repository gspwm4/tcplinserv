#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
int main()
{
    while(1){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        close(sockfd);
        perror("socket failed");
        break;
    }
    else{
        printf("socket created...\n");
    }
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bind_stat = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    char ip_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip_addr, INET_ADDRSTRLEN);

    if(bind_stat == -1){
        perror("bind failed");
        break;
    }
    else{
        printf("bind addr %s on port %d successful...\n", ip_addr, ntohs(addr.sin_port));
    }
    if(listen(sockfd, 5) == -1){
        perror("listen failed");
        break;
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
        break;
    }
    }  
    return 0;
}
