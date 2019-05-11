#include <iostream>
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"

void onPrint(std::shared_ptr<Channel> chl, Buffer& buf, Request& req)
{
    std::cout << buf.c_str();
    buf.retrieveAll();
}

void onMessage(std::shared_ptr<Channel> chl, Buffer& buf, Request& req);

int main(void)
{
    EventLoop loop;
    Channel *chl = new Channel(&loop);
    // chl->sockfd().setPort(8080);
    // chl->sockfd().listen();
    // e.g. 监听stdin
    chl->socket().setFd(0);
    chl->setReadCb(std::bind(&Channel::handelRead, chl));
    chl->setMessageCb(std::bind(&onPrint,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3));
    loop.addChannel(chl);
    loop.run();
}
