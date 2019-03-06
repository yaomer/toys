#ifndef _MYSOCK_H	
#define	_MYSOCK_H

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include    <stdarg.h>
#include	<time.h>
#include	<errno.h>
#include	<signal.h>
#include	<unistd.h>
#include	<fcntl.h>	
#include    <assert.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include    <sys/ioctl.h>
#include	<sys/stat.h>
#include	<sys/uio.h>
#include	<sys/wait.h>
#include	<sys/un.h>
#include	<sys/select.h>
#include	<poll.h>
#include	<sys/sysctl.h>
#include	<sys/ioctl.h>
#include	<sys/filio.h>
#include	<sys/sockio.h>
#include	<netinet/in.h>	
#include	<arpa/inet.h>
#include	<net/if_dl.h>
#include	<netdb.h>
#include    <sys/resource.h>

#ifndef _PTHREAD_WRAP_H
#define _PTHREAD_WRAP_H
#include    "pthread_wrap.h"
#endif

#define	LISTENQ		1024     /* 最大监听队列的长度 */
#define	MAXLINE		4096     /* 最大文本行长度 */
#define	MAXSOCKADDR  128     /* 最大套接字地址长度 */
#define	BUFFSIZE	8192     /* 缓冲区的长度 */

#define	SERV_PORT		 9877	/* 服务端口号 */
#define	SERV_PORT_STR	"9877"

#ifndef INFTIM
#define INFTIM  -1   /* poll的第三个参数 */
#endif

/* 默认创建一个新文件的权限 */
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* 默认创建一个新目录的权限 */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

/* 信号句柄函数 */
typedef	void Sigfunc(int);	

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))

ssize_t Readn(int fd, void *vptr, size_t nbytes);
void    Writen(int fd, void *vptr, size_t nbytes);
ssize_t Readline(int fd, void *vptr, size_t maxlen);
int     Tcp_listen(void);
int     Tcp_connect(const char *addr);

/* 通用错误处理函数 */
void	err_dump(const char *, ...);
void	err_msg(const char *, ...);
void	err_quit(const char *, ...);
void	err_ret(const char *, ...);
void	err_sys(const char *, ...);

#endif	/* _MYSOCK_H */
