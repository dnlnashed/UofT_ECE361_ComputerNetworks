#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    //lines 10-20 follow the same format of code as in Beej's guide
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_storage their_addr;
    char buf[100];
    char yes[3] = "yes";
    char no[2] = "no";

    socklen_t addr_len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(my_addr.sin_zero,'\0', sizeof(my_addr.sin_zero));

    int bind_check = bind(sockfd , (struct sockaddr *)&my_addr, sizeof my_addr);

    addr_len = sizeof their_addr;
    recvfrom(sockfd, buf, 99, 0, (struct sockaddr *)&their_addr, &addr_len);

    if(buf == "ftp"){
        sendto(sockfd, yes, sizeof yes, 0, (struct sockaddr *)&their_addr, sizeof their_addr);
    } else {
        sendto(sockfd, no, sizeof no, 0, (struct sockaddr *)&their_addr, sizeof their_addr);
    }

    return 0;
}