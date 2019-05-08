#include <unistd.h>
#include <sys/poll.h>
#include <errno.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"

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
    if (events() & POLLOUT) {
        if (_writeCb) _writeCb(*this);
    }
    if (events() & POLLIN) {
        if (_readCb) _readCb(*this);
    }
}
