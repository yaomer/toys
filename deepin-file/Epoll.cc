#include <sys/poll.h>
#include <vector>
#include "Epoll.h"
#include "EventLoop.h"
#include "Channel.h"

int Epoll::wait(EventLoop *loop, int64_t timeout)
{
    int nevents = poll(&_pollfds[0], _pollfds.size(), timeout);
    if (nevents > 0) {
        for (int i = 0; i < nevents; i++) {
            auto chl = loop->search(_pollfds[i].fd);
            chl.get()->setRevents(_pollfds[i].revents);
            loop->fillActiveChannel(chl);
        }
    }
    return nevents;
}
