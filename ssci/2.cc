pop3代码
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <linux/tcp.h>

#define POP3SERVPORT 110
#define MAXDATASIZE 1000
#define TRUE 1

main(int argc, char *argv[]) {
    int sockfd;
    struct hostent *host;
    struct sockaddr_in serv_addr;
    char *POPMessage[] = {
            "USER test\r\n",
            "PASS 123456\r\n",
            "STAT\r\n",
            "LIST\r\n",
            "RETR 1\r\n",
            "QUIT\r\n",
            NULL};
    int iLength;
    int iMsg = 0;
    char buf[MAXDATASIZE];
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(POP3SERVPORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.137.100");
    bzero(&(serv_addr.sin_zero), 8);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect error");
        exit(1);
    }
    iLength = recv(sockfd, buf, sizeof(buf), 0);
    buf[iLength] = '\0';
    printf("received:%s\n", buf);
    do {
        send(sockfd, POPMessage[iMsg], strlen(POPMessage[iMsg]), 0);
        printf("have sent:%s", POPMessage[iMsg]);
        iLength = recv(sockfd, buf, sizeof(buf), 0);
        buf[iLength] = '\0';
        printf("received: %s,%d\n", buf, iMsg);
        if (iMsg == 5)
            iLength = recv(sockfd, buf, sizeof(buf), 0);
        buf[iLength] = '\0';
        printf("%s\n", buf);
        iMsg++;
    } while (POPMessage[iMsg]);
    close(sockfd);
}
