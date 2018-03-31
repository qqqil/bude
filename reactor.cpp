
#include <vector>
#include <sys/epoll.h>
#include <memory>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <errno.h>

#include "logger.h"

#include "epoll_event.h"
#include "event_handler.h"
#include "accept_event_handler.h"
#include "epoll_event_loop.h"
#include "reactor.h"



Reactor::Reactor(EventLoop& evtLoop):eventLoop(evtLoop){
            logger = Logger("Reactor ");
}


void Reactor::registerHandler(EventHandler& handler){
    handlers.push_back(std::shared_ptr<EventHandler>(&handler));
}

void Reactor::handleEvents(){

    while(true){
        //get handler to handl event
        std::vector<std::shared_ptr<Event> > events = eventLoop.loop();
        logger.log("get events to handle ");
        for(std::shared_ptr<Event> evt : events){
            for(std::shared_ptr<EventHandler> handler : handlers){
                if((*handler).isSupport(*evt)){
                    (*handler).handle(*evt);
                }
            }
        }
        logger.log("handle events done! ");
    }
}


