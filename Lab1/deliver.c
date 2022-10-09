#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_storage their_addr;
    char ftp[3];
    char file[100];
    char buf[100];
    socklen_t addr_len;

    int t1 = 0;
    int t2 = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr.s_addr));
    memset(server_addr.sin_zero,'\0', sizeof(server_addr.sin_zero));

    printf("Please input a message that follows: 'ftp <file name>'\n");
    scanf("%s%s", ftp, file);

    if(access(file,F_OK) == 0) {
        t1 = clock();
        printf("Start time: %d clock ticks\n", t1);
        sendto(sockfd, ftp, strlen(ftp), 0, (struct sockaddr *)&server_addr, sizeof server_addr);
    } else {
        return 0;
    }

    addr_len = sizeof their_addr;
    recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *)&their_addr, &addr_len);
    
    t2 = clock();
    printf("End time: %d clock ticks\n", t2);
    printf("Round-trip time is: %fs\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    if(strcmp(buf, "yes") == 0) {
        printf("A file transfer can start.\n");
    } else {
        return 0;
    }

    return 0;
}