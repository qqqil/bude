#include <sys/socket.h>
#include <netinet/in.h>

#include "accept_event_handler.h"
#include "epoll_event.h"

bool AcceptHandler::isSupport(Event& evt){

    EpollEvent& eref = static_cast<EpollEvent&>(evt);

    return eref.isAccept();
}
void AcceptHandler::handle(Event& evt){
            logger.log("do accept handler");
            EpollEvent& eevt = static_cast<EpollEvent&>(evt);

            int client_fd = accept(eevt.getFd(),NULL,NULL);


            if(handleTable.find(client_fd) == handleTable.end()){
                handleTable[client_fd] = Buffer();
            }

            struct epoll_event ev;
            ev.events = EPOLLIN|EPOLLOUT;
            ev.data.fd = client_fd;
            EpollEvent *new_eevt = new EpollEvent(&ev);
            
            Event& evtRef = *new_eevt;
            EventLoop* evtLoop = eevt.getEventLoop();
            evtLoop->addEvent(evtRef);

            logger.log(new_eevt->getFd());
        }