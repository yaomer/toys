#include <iostream>
#include <unistd.h>
#include <sys/poll.h>
#include <errno.h>
#include <functional>
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"

void Channel::changeEvent(void)
{
    _loop->changeEvent(fd(), events());
}

void Channel::send(const char *s, size_t len)
{
    setStatus(Channel::SENDING);
    if (!isWriting() && _output.readable() == 0) {
        ssize_t n = write(fd(), s, len);
        if (n > 0) {
            if (n < len) {
                _output.append(s + n, len - n);
                enableWrite();
            } else {
                clearStatus(Channel::SENDING);
                if (_writeCompleteCb)
                    _writeCompleteCb();
            }
        } else {
            if (errno != EAGAIN
             && errno != EWOULDBLOCK
             && errno != EINTR)
                ;
        }
    } else
        _output.append(s, len);
}

void Channel::handleEvent(void)
{
    // 写事件由loop自动负责
    if (_revents & POLLOUT) {
        handleWrite();
    }
    // 读事件由用户负责
    if (_revents & POLLIN) {
        if (_readCb) _readCb();
    }
}

void Channel::handleAccept(void)
{
    int connfd = socket().accept();
    std::cout << connfd << " is connected" << std::endl;
    Channel *chl = new Channel(_loop);
    chl->socket().setFd(connfd);
    chl->setReadCb(std::bind(&Channel::handleRead, chl));
    chl->setMessageCb(_messageCb);
    _loop->addChannel(chl);
    // _connectionCb() for Client
}

void Channel::handleRead(void)
{
    ssize_t n = _input.readFd(fd());
    if (n > 0) {
        if (_messageCb)
            _messageCb(shared_from_this(), _input);
    } else if (n == 0) {
        handleClose();
    } else
        handleError();

}

void Channel::handleWrite(void)
{
    if (isWriting()) {
        ssize_t n = write(fd(), _output.peek(), _output.readable());
        if (n >= 0) {
            _output.retrieve(n);
            if (_output.readable() == 0) {
                clearStatus(Channel::SENDING);
                disableWrite();
                if (_writeCompleteCb)
                    _writeCompleteCb();
            }
        } else {
            // 对端已关闭连接
            if (errno == EPIPE)
                ; // close()
            if (errno != EAGAIN
             && errno != EWOULDBLOCK
             && errno != EINTR)
                ;
        }
    }
}

void Channel::handleClose(void)
{
    if (_status == Channel::SENDING) {
        // 消息发送完后再关闭连接
        setWriteCompleteCb(std::bind(&EventLoop::delChannel, _loop, this));
    } else
        _loop->delChannel(this);
}

void Channel::handleError(void)
{
    std::cout << "fd = " << fd() << " error: "
              << strerror(errno) << std::endl;
    handleClose();
}
