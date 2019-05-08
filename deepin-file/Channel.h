#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <unistd.h>
#include <sys/poll.h>
#include "Buffer.h"

class EventLoop;

class Channel {
    typedef void (*Callback)(Channel&);
    typedef void (*ReadCallback)(EventLoop *, Channel&);
public:
    explicit Channel(EventLoop *loop, int fd_)
        : _loop(loop), _fd(fd_), _readCb(NULL), 
        _writeCb(NULL), _closeCb(NULL), _errorCb(NULL), 
        _events(0), _revents(0) {  }
    ~Channel() { close(_fd); }
    int fd() { return _fd; }
    int events() { return _events; }
    void setRevents(int revents) { _revents = revents; }
    int isReading() { return _events & POLLIN; }
    int isWriting() { return _events & POLLOUT; }
    void enableRead() { _events |= POLLIN; }
    void enableWrite() { _events |= POLLOUT; changeEvent(); }
    void disableWrite() { _events &= ~POLLOUT; changeEvent(); }
    void changeEvent();
    void send(const char *s, size_t len);
    void handleEvent();
    void setReadCb(ReadCallback _cb) { _readCb = _cb; }
    void setWriteCb(Callback _cb) { _writeCb = _cb; }
    void setCloseCb(Callback _cb) { _closeCb = _cb; }
    void setErrorCb(Callback _cb) { _closeCb = _cb; }
private:
    EventLoop *_loop;
    int _fd;
    int _events;
    int _revents;
    Buffer _input;
    Buffer _output;
    ReadCallback _readCb;
    Callback _writeCb;
    Callback _closeCb;
    Callback _errorCb;
};

#endif // _CHANNEL_H
