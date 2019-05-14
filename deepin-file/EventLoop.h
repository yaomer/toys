#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <iostream>
#include <map>
#include <memory>
#include "Epoll.h"
#include "Channel.h"
#include "Timer.h"
#include "Noncopyable.h"

// 不可拷贝的
class EventLoop : Noncopyable {
public:
    // 向loop中添加一个新的Channel
    void addChannel(Channel *chl)
    {
        chl->enableRead();
        _epoll.add(chl->fd(), chl->events());
        _channelMap.insert(std::pair<int,
                std::shared_ptr<Channel>>(chl->fd(), chl));
    }
    // 从loop中移除一个Channel
    void delChannel(Channel *chl)
    {
        _epoll.del(chl->fd());
        _channelMap.erase(chl->fd());
    }
    void changeEvent(int fd, int events) 
    { 
        _epoll.change(fd, events); 
    }
    std::shared_ptr<Channel> search(int fd)
    {
        auto it = _channelMap.find(fd);
        assert(it != _channelMap.end());
        return it->second;
    }
    void fillActiveChannel(std::shared_ptr<Channel> chl) 
    { 
        _activeChannels.push_back(chl); 
    }
    void handleRead();
    // 唤醒loop
    void wakeUp();
    void runAfter(int64_t timeout, TimerCallback _cb);
    void runEvery(int64_t interval, TimerCallback _cb);
    void run();
    void quit() { _quit = 1; }
private:
    Epoll _epoll;
    std::map<int, std::shared_ptr<Channel>> _channelMap;
    std::vector<std::shared_ptr<Channel>> _activeChannels;
    Timer _timer;
    // std::unique_ptr<Timer> _timer;
    int _wakeFd[2];
    int _quit = 0;
};

#endif // _EVENTLOOP_H
