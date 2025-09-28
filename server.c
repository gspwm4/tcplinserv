#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        perror("socket failed");
        return -1;
    }
    int opt = 1;
    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))) == -1){
        perror("setsockopt failed");
        return -1;
    }
    struct sockaddr_in addr = {0};
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    char dst[INET_ADDRSTRLEN];
    if((inet_ntop(AF_INET,&addr.sin_addr.s_addr,dst,INET_ADDRSTRLEN) == NULL)){
        perror("inet ntop failed");
        close(sockfd);
        return -1;
    }
    if(bind(sockfd,(struct sockaddr *) &addr,sizeof(addr)) == -1){
        perror("bind failed");
        return -1;
    }
    if(listen(sockfd,5) == -1){
        perror("listen failed");
        return -1;
    }

    socklen_t addr_size = sizeof(addr);
    int acceptfd = accept(sockfd,(struct sockaddr *) &addr,&addr_size);
    if(acceptfd == -1){
        perror("accept failed");
        return -1;
    }
    while(1){
    char buff[1024] = {0}; 
    ssize_t read_buff = read(acceptfd,buff,sizeof(buff));
    if(read_buff <= 0){
            perror("read failed");
            break;
    }
    write(STDOUT_FILENO,buff,read_buff);
    write(acceptfd,buff,read_buff);
    }
    close(sockfd);
    close(acceptfd);
    return 0;
}
