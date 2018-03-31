#ifndef __EPOLL_EVENT_HANDLER_H
#define __EPOLL_EVENT_HANDLER_H

#include <map>


#include "event_handler.h"
#include "epoll_event.h"
#include "epoll_event_loop.h"
#include "buffer.h"

class AcceptHandler:public EventHandler{
    public:
        AcceptHandler(){
            logger = Logger("acceptHandler");
            
        }
        virtual void handle(Event& evt);
        virtual bool isSupport(Event& evt);
    private:
    Logger logger;
    std::map<int,Buffer> handleTable;

};

#endif