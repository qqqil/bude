#ifndef __REACTOR_H
#define __REACTOR_H

#include <vector>
#include <memory>

#include "logger.h"
#include "event_loop.h"
#include "event_handler.h"

class Reactor{
public:
        
        Reactor(EventLoop& evtLoop);
        void registerHandler(EventHandler& handler);
        void unregisterHandler(EventHandler& handler);
        void handleEvents();
private:
    EventLoop& eventLoop;
    std::vector<std::shared_ptr<EventHandler> > handlers;
    Logger logger;
};

#endif