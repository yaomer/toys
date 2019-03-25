#include "mysock.h"

int
main(int argc, char *argv[])
{
    int                 i, maxi, maxfd, sockfd, listenfd, connfd;
    int                 nready, client[FD_SETSIZE];
    ssize_t             n;
    fd_set              rset, allset;
    socklen_t           clilen;
    struct sockaddr_in  servaddr, cliaddr;
    char                buf[MAXLINE];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
        err_sys("bind error");
    if ((listen(listenfd, LISTENQ)) < 0)
        err_sys("listen error");

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; ++i)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for ( ; ; ) {
        rset = allset;
        if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0)
            err_sys("select error");

        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
                err_sys("accept error");

            printf("client %d connected\n", connfd);
            for (i = 0; i < FD_SETSIZE; ++i)
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            if (i == FD_SETSIZE)
                err_quit("too many clients");

            FD_SET(connfd, &allset);     /* 必须对allset进行置位 */
            maxfd = max(connfd, maxfd);
            maxi = max(i, maxi);

            if (--nready <= 0)
                continue;
        }

        for (i = 0; i <= maxi; ++i) {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else if (n > 0)
                    Writen(sockfd, buf, n);
                else
                    err_sys("read error");

                if (--nready <= 0)
                    break;
            }
        }
    }
}
