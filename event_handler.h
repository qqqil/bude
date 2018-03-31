#ifndef __EVENT_HANDLER_H
#define __EVENT_HANDLER_H

#include "logger.h"
#include "event.h"

class EventHandler{
    public:
        EventHandler(){
            _logger = Logger("event handler");
        }
        virtual void handle(Event& evt){
            _logger.log("handle");
        };
        virtual bool isSupport(Event& evt) = 0;
    private:
    Logger _logger;
};

#endif