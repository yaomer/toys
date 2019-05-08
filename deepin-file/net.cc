#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

void setNonblock(int fd)
{
    int oflag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, oflag | O_NONBLOCK);
}

int listenPort(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    socklen_t on = 1;

    signal(SIGPIPE,  SIG_IGN);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ;
    if (setsockopt(listenfd, SOL_SOCKET, TCP_NODELAY, &on, sizeof(on)) < 0)
        ;

    setNonblock(listenfd);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        ;
    if (listen(listenfd, 1024) < 0)
        ;
    return listenfd;
}

int acceptClient(int listenfd)
{
    int connfd;
    struct sockaddr_in cliaddr;
    socklen_t clilen;

    clilen = sizeof(cliaddr);
_again:
    if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
        if (errno == EINTR)
            goto _again;
        if (errno != EWOULDBLOCK /* BSD */
         && errno != EPROTO  /* SERV4 */
         && errno != ECONNABORTED)  /* POSIX */
            ;
    }
    return connfd;
}
