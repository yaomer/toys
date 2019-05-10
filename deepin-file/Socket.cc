#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Socket.h"

void Socket::setNonblock(void)
{
    int oflag = fcntl(_sockfd, F_GETFL, 0);
    fcntl(_sockfd, F_SETFL, oflag | O_NONBLOCK);
}

void Socket::setReuseAddr(void)
{
    socklen_t on = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ;
}

void Socket::setNodelay(void)
{
    socklen_t on = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, TCP_NODELAY, &on, sizeof(on)) < 0)
        ;
}

void Socket::listen(void)
{
    struct sockaddr_in servaddr;
    socklen_t on = 1;

    signal(SIGPIPE, SIG_IGN);

    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ;

    setNonblock();
    setReuseAddr();
    setNodelay();

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(_port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        ;
    if (::listen(_sockfd, LISTENQ) < 0)
        ;
}

int Socket::accept(void)
{
    struct sockaddr_in cliaddr;
    socklen_t clilen;

    clilen = sizeof(cliaddr);
    int connfd = ::accept(_sockfd, (struct sockaddr *)&cliaddr, &clilen);
_again:
    if (connfd < 0) {
        if (errno == EINTR)
            goto _again;
        if (errno != EWOULDBLOCK /* BSD */
         && errno != EPROTO  /* SERV4 */
         && errno != ECONNABORTED)  /* POSIX */
            ;
    }
    setNonblock();
    return connfd;
}
