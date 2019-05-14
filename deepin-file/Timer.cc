#include <sys/time.h>
#include <iostream>
#include <memory>
#include "Timer.h"

int64_t Timer::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Timer::tick()
{
    if (!_timer.empty()) {
        const Timestamp *t = get();
        t->timerCb()();
        if (t->interval() > 0) {
            Timestamp *_t = new Timestamp;
            _t->setTimeout(t->interval());
            _t->setInterval(t->interval());
            _t->setTimerCb(t->timerCb());
            add(_t);
        }
        pop();
    }
}
