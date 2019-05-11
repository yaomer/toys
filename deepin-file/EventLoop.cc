#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

void EventLoop::run(void)
{
    while (!_quit) {
        int nevents = _epoll.wait(this, -1);
        if (nevents > 0) {
            auto end = _activeChannels.end();
            for (auto x = _activeChannels.begin(); x != end; x++)
                (*x).get()->handleEvent();
                // (*x)->handleEvent();
            _activeChannels.clear();
        } else if (nevents == 0)
            ; // timer
        else
            ; // error
    }
}
