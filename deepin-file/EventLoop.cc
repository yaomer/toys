#include <sys/socket.h>
#include <functional>
#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "Timer.h"

void EventLoop::addWakeChannel(void)
{
    socketpair(AF_LOCAL, SOCK_STREAM, 0, _wakeFd);
    Channel *chl = new Channel(this);
    chl->socket().setFd(_wakeFd[0]);
    chl->setReadCb(std::bind(&EventLoop::handleRead, this));
    addChannel(chl);
}

void EventLoop::run(void)
{
    addWakeChannel();

    while (!_quit) {
        int nevents = _poller->wait(this, _timer.timeout());
        if (nevents > 0) {
            for (auto& it : _activeChannels)
                it.get()->handleEvent();
            _activeChannels.clear();
        } else if (nevents == 0)
            _timer.tick();
        else
            ; // error
    }
}

void EventLoop::wakeUp(void)
{
    uint64_t one = 1;
    write(_wakeFd[1], &one, sizeof(one));
}

void EventLoop::handleRead(void)
{
    uint64_t one;
    read(_wakeFd[0], &one, sizeof(one));
}

void EventLoop::runAfter(int64_t timeout, TimerCallback _cb)
{
    Timestamp *t = new Timestamp;
    t->setTimeout(timeout);
    t->setInterval(0);
    t->setTimerCb(_cb);
    _timer.add(t);
    wakeUp();
}

void EventLoop::runEvery(int64_t interval, TimerCallback _cb)
{
    Timestamp *t = new Timestamp;
    t->setTimeout(interval);
    t->setInterval(interval);
    t->setTimerCb(_cb);
    _timer.add(t);
    wakeUp();
}
