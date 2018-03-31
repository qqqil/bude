#ifndef __EPOLL_EVENT_LOOP_H
#define __EPOLL_EVENT_LOOP_H

#include <vector>
#include <sys/epoll.h>
#include <memory>
#include "event_loop.h"
#include "logger.h"
#include "epoll_event.h"
#include <map>

class EpollEventLoop:public EventLoop{
    public: 
        EpollEventLoop();
        virtual ~EpollEventLoop(){
            
        }
    std::vector<std::shared_ptr<Event> > loop();
    void addEvent(Event& evt);
    void removeEvent(Event& evt);
    private:
        struct epoll_event* eevents;      
        const int MAX_EVENTS = 64;
        int epollfd;
        Logger logger;
        std::map<int,int> handleMap;
};

#endif
