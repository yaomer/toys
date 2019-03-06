#include "mysock.h"

ssize_t
readn(int fd, void *vptr, size_t n)
{
    size_t  nleft;    /* 当前剩余字符数 */
    size_t  nread;    /* 每次read()读取的字符数 */
    char    *ptr;

    ptr   = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)   /* 中断后手动重启 */
                nread = 0;
            else
                return -1;
        } else if (nread == 0)   /* 读到EOF */
            break;
        nleft -= nread;
        ptr   += nread;
    }
    return (n - nleft);    /* 返回已读字符数 */
}

ssize_t
writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; ++n) {
again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            *ptr = '\0';
            return (n - 1);
        } else {
            if (errno == EINTR)
                goto again;
            return -1;
        }
    }
    *ptr = '\0';
    return n;
}

ssize_t
Readn(int fd, void *vptr, size_t nsize)
{
    ssize_t     n;

    if ((n = readn(fd, vptr, nsize)) < 0)
        err_sys("readn error");
    return n;
}

void
Writen(int fd, void *vptr, size_t nsize)
{
    if (writen(fd, vptr, nsize) != nsize)
        err_sys("writen error");
}

ssize_t
Readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t     len;

    if ((len = readline(fd, vptr, maxlen)) < 0)
        err_sys("readline error");
    return len;
}

int
tcp_listen(void)
{
    int                 listenfd;
    struct sockaddr_in  servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("bind error");

    if (listen(listenfd, LISTENQ) < 0)
        err_sys("listen error");
    return listenfd;
}

int
tcp_connect(const char *addr)
{
    int                 connfd;
    struct sockaddr_in  servaddr;

    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, addr, (void *)&servaddr.sin_addr) <= 0)
        err_sys("inet_pton error");

    if (connect(connfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");
    return connfd;
}

int
Tcp_listen(void)
{
    return tcp_listen();
}

int
Tcp_connect(const char *addr)
{
    return tcp_connect(addr);
}
