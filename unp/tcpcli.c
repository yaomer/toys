#include "mysock.h"

void str_cli(FILE *fp, int sockfd);

int main(int argc, char *argv[])
{
    int                 sockfd;
    struct sockaddr_in  servaddr;

    if (argc != 2)
        err_quit("usage: a.out <IPaddress>");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], (void *)&servaddr.sin_addr) <= 0)
        err_sys("inet_pton error");

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");

    str_cli(stdin, sockfd);
    exit(0);
}

/*
 * Fgets读入一行文本，Writen把它发送给服务器
 * Readline从服务器读入回射行，Fputs把它写到标准输出
 */
void
str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL && !ferror(fp)) {
        Writen(sockfd, sendline, strlen(sendline));

        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");

        fputs(recvline, stdout);
    }
}
