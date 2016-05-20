//
// Created by qqqil on 16-5-13.
//

#ifndef POLLTEST_EPOLLEVENT_H
#define POLLTEST_EPOLLEVENT_H


#include <sys/epoll.h>
#include "Event.h"
#include "Logger.h"

class EpollEvent : public Event{

public:
    EpollEvent(int _fd) : Event(_fd),logger("EpollEvent") {
//        data.events= EPOLLIN|EPOLLOUT |EPOLLET;//read and write
    }
   ~EpollEvent(){
       logger.info("destroy epoll event");
       close(data.data.fd);
   }
    virtual int get_event_type() override {
        return 0;
    }




    virtual event_data& get_event() override {
        return data;
    }

private:
    Logger logger;
};


#endif //POLLTEST_EPOLLEVENT_H
