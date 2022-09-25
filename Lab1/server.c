#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    //lines 10-20 follow the same format of code as in Beej's guide
    int sockfd;
    struct sockaddr_in my_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(*argv));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(my_addr.sin_zero,'\0', sizeof(my_addr.sin_zero));

    int bind_check = bind(sockfd , (struct sockaddr *)&my_addr, sizeof my_addr);



    return 0;
}