#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

struct packet {
    unsigned int total_frag;
    unsigned int frag_no;
    unsigned int size;
    char* filename;
    char filedata[1000];
};


int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_storage their_addr;
    char buf[1100];
    socklen_t addr_len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(my_addr.sin_zero,'\0', sizeof(my_addr.sin_zero));

    int bind_check = bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr);

    const char s[2] = ":";
    FILE *fp;
    char destination[12] = "test_files/";

    int i = 0;
    do {
        char *buff = malloc(sizeof(char) * 1100);
        struct packet packets = {0,0,0};

        addr_len = sizeof (struct sockaddr_storage);
        int bytes = recvfrom(sockfd, buff, 1100, 0, (struct sockaddr *)&their_addr, &addr_len);

        int index = 0;
        while(buff[index] != ':') {
            int digit = buff[index] - '0';
            packets.total_frag = packets.total_frag * 10 + digit;
            index++;
        }

        index++;
        while(buff[index] != ':') {
            int digit = buff[index] - '0';
            packets.frag_no = packets.frag_no * 10 + digit;
            index++;
        }

        index++;
        while(buff[index] != ':') {
            int digit = buff[index] - '0';
            packets.size = packets.size * 10 + digit;
            index++;
        }

        index++;
        char *index_name = malloc(sizeof(char));
        int counter = 0;
        while(buff[index] != ':') {
            index_name = realloc(index_name, sizeof(char)*counter + 1);
            index_name[counter] = buff[index];
            index++;
            counter++;
        }
        packets.filename = index_name;

        index++;
        if(packets.frag_no != packets.total_frag) {
            for(int v = 0; v < packets.size + 1; v++) {
                packets.filedata[v] = buff[index + v];
            }
        } else {
             for(int v = 0; v < packets.size; v++) {
                packets.filedata[v] = buff[index + v];
            }           
        }

        if(packets.frag_no == 1) {
             strcat(destination,packets.filename);
             fp = fopen(destination, "wb");            
        }

        fwrite(packets.filedata, 1, packets.size, fp);
        
    
        if(bytes != -1){
            sendto(sockfd, "yes", 4, 0, (struct sockaddr *)&their_addr, sizeof their_addr);
        } else {  
            sendto(sockfd, "no", 3, 0, (struct sockaddr *)&their_addr, sizeof their_addr);
        }

        if(packets.total_frag == packets.frag_no)
            i = 1;

    } while ( i == 0);

    fclose(fp);

    return 0;
}