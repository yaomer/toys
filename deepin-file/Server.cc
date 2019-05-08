#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "net.h"

int main(void)
{
    EventLoop loop;
    int listenfd = listenPort(8080);
    loop.addChannel(listenfd);
    loop.run();
}
