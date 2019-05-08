#include "Channel.h"
#include "EventLoop.h"
#include "net.h"

void handelRead(Channel& chl)
{

}

void handleWrite(Channel& chl)
{

}

void handleClose(Channel& chl)
{

}

void handleError(Channel& chl)
{

}

void handleAccept(EventLoop *loop, Channel& chl)
{
    int connfd = acceptClient(chl.fd());
    loop->addChannel(connfd);
}
