#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <iostream>
#include <map>
#include "Epoll.h"
#include "Channel.h"

class EventLoop {
public:
    EventLoop() : _quit(0) {  }
    ~EventLoop() {  }
    void addChannel(Channel *chl)
    {
        chl->enableRead();
        _epoll.add(chl->fd(), chl->events());
        _map.insert(std::pair<int, Channel*>(chl->fd(), chl));
    }
    void delChannel(Channel *chl)
    {
        _epoll.del(chl->fd());
        _map.erase(chl->fd());
    }
    void changeEvent(int fd, int events) { _epoll.change(fd, events); }
    Channel *search(int fd) 
    { 
        auto it = _map.find(fd);
        assert(it != _map.end());
        return it->second;
    } 
    void fillActiveChannel(Channel *chl) { _activeChannels.push_back(chl); }
    void run() 
    {  
        while (!_quit) {
            int nevents = _epoll.wait(this, -1);
            if (nevents > 0) {
                auto end = _activeChannels.end();
                for (auto x = _activeChannels.begin(); x != end; x++)
                    (*x)->handleEvent();
                _activeChannels.clear();
            } else if (nevents == 0)
                ; // timer
            else
                ; // error
        }
    }
    void quit() { _quit = 1; }
private:
    Epoll _epoll;
    std::map<int, Channel*> _map;
    std::vector<Channel*> _activeChannels;
    int _quit;
};

#endif // _EVENTLOOP_H
