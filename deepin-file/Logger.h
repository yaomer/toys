#ifndef _LOGGER_H
#define _LOGGER_H

#include <pthread.h>
#include "Buffer.h"

class EventLoop;

class Logger {
public:
    explicit Logger(EventLoop *loop);
    ~Logger();
    enum level {
        DEBUG   = 001,
        WARN    = 002,
        ERROR   = 004,
    };
    void output(const char *level, const char *_FILE_,
        int _LINE_, const char *_func_, const char *fmt, ...);
private:
    void writeToBuffer(const char *s, size_t len);
    static void *flushToFile(void *arg);
    void wakeUp();
    EventLoop *_loop;
    Buffer _writeBuf;
    Buffer _flushBuf;
    pthread_t _tid;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    int _quit; 
};

extern Logger *_log;

#define logDebug(...) \
    _log->output("DEBUG: ", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define logWarn(...) \
    _log->output("WARN:  ", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define logError(...) \
    _log->output("ERROR: ", __FILE__, __LINE__, __func__, __VA_ARGS__)

#endif // _LOGGER_H
