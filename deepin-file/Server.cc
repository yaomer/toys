#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "net.h"

int main(void)
{
    EventLoop loop;
    Channel *chl = new Channel(&loop, 0);
    chl->setReadCb(chl->handleAccept);
    chl->setMessageCb(chl->handleMessage);
    chl->setCloseCb(chl->handleClose);
    chl->setErrorCb(chl->handleError);
    loop.addChannel(chl);
    loop.run();
}
