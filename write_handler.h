#ifndef __WRITE_HANDLER_H
#define __WRITE_HANDLER_H

#include <map>

#include "logger.h"
#include "event.h"
#include "buffer.h"
#include "event_handler.h"

class WriteHandler:public EventHandler{
    public:
    virtual void handle(Event& evt);
    virtual bool isSupport(Event& evt);
    private:
    Logger logger;
    std::map<int,Buffer> handleTable;
};

#endif