#include <iostream>
#include <memory>
#include <functional>
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"

void onPrint(std::shared_ptr<Channel> chl, Buffer& buf)
{
    std::cout << buf.c_str();
    buf.retrieveAll();
}

void printStr(const char *s)
{
    std::cout << s << std::endl;
}

void onMessage(std::shared_ptr<Channel> chl, Buffer& buf);

int main(void)
{
    EventLoop loop;
    Channel *chl = new Channel(&loop);
    chl->socket().setPort(8888);
    chl->socket().listen();
    std::cout << "Server is listening port 8888" << std::endl;
    // e.g. 监听stdin
    // chl->socket().setFd(0);
    chl->setReadCb(std::bind(&Channel::handleAccept, chl));
    chl->setMessageCb(std::bind(&onMessage,
                std::placeholders::_1,
                std::placeholders::_2));
    loop.addChannel(chl);
    // e.g. timer
    // loop.runEvery(200, std::bind(&printStr, "hello, world"));
    // loop.runEvery(800, std::bind(&printStr, "1234567"));
    loop.run();
}
