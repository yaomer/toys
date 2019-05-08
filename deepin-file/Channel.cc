#include <iostream>
#include <unistd.h>
#include <sys/poll.h>
#include <errno.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "net.h"

void Channel::changeEvent(void)
{
    _loop->changeEvent(fd(), events());
}

void Channel::send(const char *s, size_t len)
{
    if (!isWriting() && _output.readable() == 0) {
        ssize_t n = write(fd(), s, len);
        if (n > 0) {
            if (n < len) {
                _output.append(s + n, len - n);
                enableWrite();
            } else {
                ; // writeCompleteCb;
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
    if (_revents & POLLOUT) {
        if (_writeCb) _writeCb(*this);
    }
    if (_revents & POLLIN) {
        if (_readCb) _readCb(*this);
    }
}

void Channel::handelRead(Channel& chl)
{
    ssize_t n = chl._input.readFd(chl.fd());
    if (n > 0) {
        if (chl._messageCb)
            chl._messageCb(chl);
    } else if (n == 0) {
        if (chl._closeCb)
            chl._closeCb(chl);
    } else {
        if (chl._errorCb)
            chl._errorCb(chl);
    }
}

void Channel::handleWrite(Channel& chl)
{
    if (chl.isWriting()) {
        ssize_t n = write(chl.fd(), chl._output.peek(), chl._output.readable());
        if (n >= 0) {
            chl._output.retrieve(n);
            if (chl._output.readable() == 0) {
                chl.disableWrite();
                // writeCompleteCb()
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

void Channel::handleClose(Channel& chl)
{
    chl._loop->delChannel(&chl);
}

void Channel::handleError(Channel& chl)
{

}

void Channel::handleAccept(Channel& _chl)
{
    int connfd = acceptClient(_chl.fd());
    Channel *chl = new Channel(_chl._loop, connfd);
    chl->setReadCb(chl->handelRead);
    chl->setWriteCb(chl->handleWrite);
    chl->setMessageCb(_chl._messageCb);
    chl->setCloseCb(chl->handleClose);
    chl->setErrorCb(chl->handleError);
    _chl._loop->addChannel(chl);
}
