#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int sockfd=0,connfd=0;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr = {.sin_port = htons(5000), .sin_family = AF_INET, .sin_addr.s_addr = htonl(INADDR_ANY)};
    struct sockaddr_in client_addr;
    socklen_t len = (int)sizeof(client_addr);

    bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    listen(sockfd,1);
    connfd = accept(sockfd,(struct sockaddr *)&client_addr,&len); //this sleeps till connection received
    write(connfd,"Hi Bitch",9);
    printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("port is: %d\n", (int) ntohs(client_addr.sin_port));
    close(connfd);
}