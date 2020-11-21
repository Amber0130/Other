smtp代码
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdbool.h>
#include<linux/tcp.h>

#define POP3SERVPORT 25
#define MAXDATASIZE 4096
#define TRUE 1

main(int argc, char *argv[]) {
    int sockfd;
    struct hostent *host;
    struct sockaddr_in serv_addr;
    char *POPMessage[] = {
            "helo\r\n",
            "mailfrom:<root@zhangshichen.com>\r\n",
            "rcptto:<zhangshichen@zhangshichen.com>\r\n",
            "data\r\n",
            "Thisis a test\r\n.\r\n",
            "QUIT\r\n",
            NULL
    };
    int iLength = 0;
    int iMsg = 0;
    char buf[MAXDATASIZE];
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socketerror");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(POP3SERVPORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.137.100");
    bzero(&(serv_addr.sin_zero), 8);/*置指针变量(&(serv_addr.sin_zero)中前8个变量的值为零*/
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("connecterror");
        exit(1);
    }
    iLength = recv(sockfd, buf, sizeof(buf), 0);
    buf[iLength] = '\0';
    printf("received:%s\n", buf);//依次发送SMPT命令，发送邮件
    do {
        bool bNodelay = TRUE;
        /*bool型变量只有两个值：false和true，是0和1的区别*/
        setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *) &bNodelay, sizeof(bool));
        send(sockfd, POPMessage[iMsg], strlen(POPMessage[iMsg]), 0);
        printf("havesent: %s", POPMessage[iMsg]);
        iLength = recv(sockfd, buf, sizeof(buf), 0);
        buf[iLength] = '\0';
        iMsg++;
        printf("received:%s,%d\n", buf, iMsg);
    } while (POPMessage[iMsg]);
    close(sockfd);
}
