#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

void EventLoop::run(void)
{
    while (!_quit) {
        int nevents = _epoll.wait(this, -1);
        if (nevents > 0) {
            for (auto& it : _activeChannels)
                it.get()->handleEvent();
            _activeChannels.clear();
        } else if (nevents == 0)
            ; // timer
        else
            ; // error
    }
}
