/* client code of a half duplex chat program        */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT      4950
#define MAXBUFLEN   100


int main(int argc, char *argv[])
{
        int sockfd;
        struct sockaddr_in their_addr;
        struct hostent *he;
        int numbytes, addr_len;
        char buf[MAXBUFLEN];
        char arr[MAXBUFLEN];


        if (argc != 2) {
                fprintf(stderr, "usage: talker hostname message\n");
                exit(1);
        }

        if ((he = gethostbyname(argv[1])) == NULL) {
                perror("gethostbyname");
                exit(1);
        }

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                 perror("socket");
                 exit(1);
        }

        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(MYPORT);
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(&(their_addr.sin_zero), '\0', 8);

        while(1) {
                 printf("Type the message: \n");
                 gets(arr);
                 if ((numbytes = sendto(sockfd, arr, strlen(arr), 0,
                                        (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
                     perror("sendto");
                     exit(1);
                }
                printf("send %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));
                 if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                        perror("recv from server");
                         exit(1);
                }
                buf[numbytes] = '\0';
                printf("message: %s\n", buf);
        }
                close(sockfd);

                return 0;
}

