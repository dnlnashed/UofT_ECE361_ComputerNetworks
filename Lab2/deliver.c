#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>

struct packet {
    unsigned int total_frag; 
    unsigned int frag_no; 
    unsigned int size;  
    char* filename;
    char filedata[1000];
};

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct sockaddr_storage their_addr;
    char ftp[3];
    char file[100]; 
    char buf[100];
    socklen_t addr_len;

    FILE *fp;
    char buffer[1000];
    for(int p = 0; p < 1000; p++)
        buffer[p] = '\0';
    struct stat st;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr.s_addr));
    memset(server_addr.sin_zero,'\0', sizeof(server_addr.sin_zero));

    printf("Please input a message that follows: 'ftp <file name>'\n");
    scanf("%s%s", ftp, file);

    if(access(file,F_OK) == 0) {
        fp = fopen(file, "rb");
        stat(file, &st);
        
        int counter = (st.st_size+1000)/1000;
        int byte_tracker = st.st_size;
        int buffer_size = 0;

        for(int i = 1; i <= counter; i++) {
            char *message = malloc(sizeof(char) * 1100);

            if(byte_tracker > 1000) {
                byte_tracker = byte_tracker - 1000;
                buffer_size = 1000;
            } else {
                buffer_size = byte_tracker;
            }
    
            int size = fread(buffer, 1, 1000, fp);
            struct packet packets = {counter, i, size, file};
            memcpy(packets.filedata,buffer,buffer_size);

            int index = 0;
            int current_index = 0;
            int digits = 1;

            int copy = packets.total_frag;
            while(copy >= 10) {
                copy = copy/10;
                digits++;
            }

            current_index = index;
            copy = packets.total_frag;
            for(int a = 0; a < digits; a++) {
                int digit = copy % 10;
                message[(digits+current_index) - a - 1] = (char)digit + '0';
                copy = copy/10;
                index++;
            }

            message[index] = ':';
            index++;

            digits = 1;
            copy = packets.frag_no;
            while(copy >= 10) {
                copy = copy/10;
                digits++;
            }

            current_index = index;
            copy = packets.frag_no;
            for(int b = 0; b < digits; b++) {
                int digit = copy % 10;
                message[(digits+current_index) - b - 1] = (char)digit + '0';
                copy = copy/10;
                index++;
            }           

            message[index] = ':';
            index++;

            digits = 1;
            copy = packets.size;
            while(copy >= 10) {
                copy = copy/10;
                digits++;
            }

            current_index = index;
            copy = packets.size;
            for(int c = 0; c < digits; c++) {
                int digit = copy % 10;
                message[(digits+current_index) - c - 1] = (char)digit + '0';
                copy = copy/10;
                index++;
            }       

            message[index] = ':';
            index++;

            memcpy(&message[index], packets.filename, strlen(packets.filename));
            index = index + strlen(packets.filename);

            message[index] = ':';
            index++;

            for(int v = 0; v < packets.size; v++) {
                message[index + v] = packets.filedata[v];
            }

            sendto(sockfd, message , 1100, 0, (struct sockaddr *)&server_addr, sizeof server_addr);

            addr_len = sizeof their_addr;
            recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *)&their_addr, &addr_len);

            if(strcmp(buf, "yes") == 0) {
                printf("Acknowledgement: Packet %d has been received!\n", packets.frag_no);
            } else {
                return 0;
            }

        } 
    } else {
        printf("File does not exist!\n");
        return 0;
    }

    fclose(fp);

    return 0;
}