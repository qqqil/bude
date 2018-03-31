#ifndef __EPOLL_EVENT_H
#define __EPOLL_EVENT_H

#include <sys/epoll.h>

#include "event.h"
#include "logger.h"
#include "event_loop.h"

class EpollEvent:public Event{
    public:

    EpollEvent(struct epoll_event* evt):eevent(evt){
        logger = Logger("epoll event");
        type = 0;
        
        logger.log("ctor @");
        logger.log(getFd());
    }

    EpollEvent(const EpollEvent& evt){
        type = 0;
        
        logger.log("copy tor@");
        logger.log(this);
    }
    virtual void* getData();

    ~EpollEvent();

    void setEventLoop(EventLoop* evtloop);
    void setType(int evtType);
    int getType();
    int getFd();
    bool isAccept();
    bool isRead();
    bool isWrite();

    EventLoop* getEventLoop();
    private:
        struct epoll_event *eevent;
        EventLoop* eventLoop;
        int type;
        Logger logger;
};

#endif /**/