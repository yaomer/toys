/*
 * 总结：
 * 1. 僵死进程，需处理SIGCHLD信号
 * 2. 信号处理函数必须使用waitpid函数而非wait函数，因为Unix信号是不排队的
 * 3. 当服务器进程终止时，客户进程没有被告知，因为此时它正阻塞于Fgets
 * 4. 服务主机崩溃的情形要等到客户向服务器发送了数据后才能监测到
 * 5. 简单的交换文本行没有什么问题，但二进制数据在不同机器间(字节序或类型长度)
 * 却会出现问题
 */
#include "mysock.h"

void str_echo(int sockfd);
void sig_chld(int);

int
main(int argc, char *argv[])
{
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  servaddr, cliaddr;

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

    signal(SIGCHLD, sig_chld);
    /* 当子进程终止时，内核会向其父进程发送SIGCHLD信号
     * 而该信号默认会被忽略，如果不加处理，就会导致子进程成为
     * 僵死进程，因此我们需要手动处理该信号。
     *
     * signal函数用于捕获SIGCHLD信号，并作相应地处理
     */

    for ( ; ; ) {
        clilen = sizeof(cliaddr);

        /* 当我们手动捕获SIGCHLD信号时，被阻塞的accept之类的慢速系统调用
         * 就有可能被中断，因此我们需要手动重启
         */
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
            if (errno == EINTR)
                continue;
            else
                err_sys("accept error");
        }

        if ((childpid = fork()) == 0) {
            close(listenfd);
            str_echo(connfd);
            exit(0);
        } else if (childpid < 0)
            err_sys("fork error");

        close(connfd);
    }
}

/* 对客户端发送的两个数进行求和，然后将结果回射给客户端 */
/* void
str_echo(int sockfd)
{
    int     arg1, arg2;
    char    line[MAXLINE];

    for ( ; ; ) {
        if (Readline(sockfd, line, MAXLINE) == 0)
            return;

        if (sscanf(line, "%d%d", &arg1, &arg2) == 2)
            snprintf(line, sizeof(line), "%d\n", arg1 + arg2);
        else
            snprintf(line, sizeof(line), "input error\n");
        Writen(sockfd, line, strlen(line));
    }
} */

void
str_echo(int sockfd)
{
    ssize_t     n;
    char        buf[MAXLINE];

again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0)
        Writen(sockfd, buf, n);

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        err_sys("str_echo: read error");
}

void
sig_chld(int signo)
{
    pid_t   pid;
    int     stat;

    /* -1参数表示等待第一个终止的子进程
     * 循环使用waitpid以保证所有子进程正常终止，防止出现
     * 僵死进程
     *
     * WNOHANG选项告知waitpid在有尚未终止的子进程在运行时
     * 不要阻塞
     */
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
}
