#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <sys/poll.h>
#include "Buffer.h"
#include "Coder.h"
#include "Socket.h"
#include "Noncopyable.h"

class EventLoop;

// 不可拷贝的
class Channel : Noncopyable, 
    public std::enable_shared_from_this<Channel>  {
public:
    typedef std::function<void(std::shared_ptr<Channel>, Buffer&, 
            Request&)> MessageCallback;
    typedef std::function<void()> ReadCallback;
    explicit Channel(EventLoop *loop) : _loop(loop) {  }
    ~Channel() {  }
    int fd() { return _socket.fd(); }
    Socket& socket() { return _socket; }
    int events() { return _events; }
    void setRevents(int revents) { _revents = revents; }
    int isReading() { return _events & POLLIN; }
    int isWriting() { return _events & POLLOUT; }
    void enableRead() { _events |= POLLIN; }
    void enableWrite() { _events |= POLLOUT; changeEvent(); }
    void disableWrite() { _events &= ~POLLOUT; changeEvent(); }
    void changeEvent();
    void send(const char *s, size_t len);
    void setReadCb(const ReadCallback _cb) { _readCb = _cb; }
    void setMessageCb(const MessageCallback _cb) { _messageCb = _cb; }
    void handleEvent();
    void handelRead();
    void handleWrite();
    void handleClose();
    void handleError();
    void handleAccept();
private:
    EventLoop *_loop;
    Socket _socket;
    int _events = 0;
    int _revents = 0;
    Buffer _input;
    Buffer _output;
    Request _req;
    ReadCallback _readCb = nullptr;
    MessageCallback _messageCb = nullptr;
};

#endif // _CHANNEL_H
