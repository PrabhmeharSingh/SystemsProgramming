#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int sockfd=0;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr = {.sin_port = htons(5000), .sin_family = AF_INET, .sin_addr.s_addr = inet_addr("127.0.0.1")};
    printf("Trying To Connect");
    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    char str[9];
    read(sockfd,str,9);
    printf("%s",str);
    close(sockfd);
}