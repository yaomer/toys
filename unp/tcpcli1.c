/*
 * 使用select的FD_ISSET测试宏时要特别注意
 * 每次测试时，描述符集内任何与未就绪描述符对应的位返回时将被清0
 * 因此，每次重新调用select函数时，都需要把我们关心的位均置为1
 */
#include "mysock.h"

void str_cli(FILE *fp, int sockfd);

int
main(int argc, char *argv[])
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

    printf("client %d\n", sockfd);
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
    int     maxfd;       /* select监听的最大描述符数量 */
    int     stdineof;    /* 标识是否从fp中读到了EOF */
    fd_set  rset;
    char    buf[MAXLINE];
    int     n;

    stdineof = 0;
    FD_ZERO(&rset);
    for ( ; ; ) {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfd = max(fileno(fp), sockfd) + 1;

        if (select(maxfd, &rset, NULL, NULL, NULL) < 0)
            err_sys("select error");

        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1)
                    return;
                else
                    err_quit("str_cli: server terminated prematurely");
            } else if (n < 0)
                err_sys("read error");
            Writen(2, buf, n);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                /* 半关闭：只关闭sockfd写端，表示不再向sockfd中写入数据，但仍可能
                 * 从sockfd中读入数据，此时客户会向服务器发送一个FIN分节
                 */
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            } else if (n < 0)
                err_sys("read error");
            Writen(sockfd, buf, n);
        }
    }
}
