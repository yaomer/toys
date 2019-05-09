#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "net.h"

int main(void)
{
    EventLoop loop;
    int listenfd = listenPort(8080);
    Channel *chl = new Channel(&loop, listenfd);
    chl->setReadCb(chl->handleAccept);
    chl->setMessageCb(chl->handleMessage);
    chl->setCloseCb(chl->handleClose);
    chl->setErrorCb(chl->handleError);
    loop.addChannel(chl);
    loop.run();
}
