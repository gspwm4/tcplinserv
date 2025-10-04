#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
    signal(SIGPIPE, SIG_IGN);
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
        close(sockfd);
        perror("inet ntop failed");
        return -1;
    }
    if(bind(sockfd,(struct sockaddr *) &addr,sizeof(addr)) == -1){
        close(sockfd);
        perror("bind failed");
        return -1;
    }
    if(listen(sockfd,5) == -1){
        close(sockfd);
        perror("listen failed");
        return -1;
    }
    while(1){
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t addr_size = sizeof(client_addr);
    int acceptfd = accept(sockfd,(struct sockaddr *) &client_addr,&addr_size);
    if(acceptfd == -1){
        perror("accept failed");
        continue;
    }
    while(1){
    char buff[1024] = {0};
    ssize_t read_buff = read(acceptfd,buff,sizeof(buff)-1);
    if(read_buff == -1){
        perror("read failed");
        return -1;
    }
    if(read_buff == 0){
        printf("client disconnected\n");
        break;
    }
    printf("Receive from client: ");
    fflush(stdout);
    ssize_t rec_output = write(STDOUT_FILENO,buff,read_buff);
    if(rec_output == -1){
        perror("output write failed");
        return -1;
    }
    ssize_t acceptfd_write = write(acceptfd,buff,read_buff);
    if(acceptfd_write == -1){
        perror("acceptfd write failed");
        return -1;
    }
    buff[read_buff] = '\0';
    }
    close(acceptfd);
    }
    close(sockfd);
    return 0;
}
