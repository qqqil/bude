#ifndef __EVENT_LOOP_H
#define __EVENT_LOOP_H

#include "event.h"
#include <vector>
#include <memory> 

class EventLoop{
    public:
        EventLoop(){}
        virtual ~EventLoop() {} ;
        virtual std::vector<std::shared_ptr<Event> > loop()=0;
        virtual void addEvent(Event& evt)=0;
        virtual void removeEvent(Event& evt)=0;
};

#endif