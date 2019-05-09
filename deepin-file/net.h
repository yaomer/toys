#ifndef _NET_H
#define _NET_H

void setNonblock(int fd);
int listenPort(int port);
int acceptClient(int listenfd);

#endif // _NET_H
