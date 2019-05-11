#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <iostream>
#include <map>
#include "Epoll.h"
#include "Channel.h"
#include "Noncopyable.h"

// 不可拷贝的
class EventLoop : Noncopyable {
public:
    // 向loop中添加一个新的Channel
    void addChannel(Channel *chl)
    {
        chl->enableRead();
        _epoll.add(chl->fd(), chl->events());
        // auto it = std::make_shared<Channel>(chl);
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
    void run();
    void quit() { _quit = 1; }
private:
    Epoll _epoll;
    std::map<int, std::shared_ptr<Channel>> _channelMap;
    // std::map<int, Channel*> _map;
    std::vector<std::shared_ptr<Channel>> _activeChannels;
    // std::vector<Channel*> _activeChannels;
    int _quit = 0;
};

#endif // _EVENTLOOP_H
