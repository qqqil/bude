
#include <vector>
#include <sys/epoll.h>
#include <memory>
#include "epoll_event_loop.h"
EpollEventLoop::EpollEventLoop(){
            epollfd = epoll_create(1);
            if(epollfd == -1){
                logger.log("create epoll fd error");
                
            }
            eevents = new epoll_event[MAX_EVENTS];
        }
void EpollEventLoop::addEvent(Event& evt){
    struct epoll_event * eevt =(struct epoll_event*)evt.getData();
    int fd=eevt->data.fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,eevt);
    logger.log("add event");
    EpollEvent& eref = static_cast<EpollEvent&>(evt);
    int type = eref.getType();
    if(handleMap.find(fd) == handleMap.end()){
        handleMap[fd] = type;
    }
}

void EpollEventLoop::removeEvent(Event& evt){
    struct epoll_event * eevt = (struct epoll_event*)evt.getData();
    int fd=eevt->data.fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,eevt);
    //remove from ctrl map 
    std::map<int,int>::iterator itr = handleMap.find(fd);
    if(itr != handleMap.end()){
        handleMap.erase(itr);
    }
}


std::vector<std::shared_ptr<Event> > EpollEventLoop::loop(){
    std::vector<std::shared_ptr<Event> > evts;
    logger.log("start event wait..");
    
    int nfds = epoll_wait(epollfd,eevents,MAX_EVENTS,-1);
    if(nfds == -1){
        return evts;
    }
    
    for(int i=0;i<nfds;++i){
        EpollEvent *evt = new EpollEvent(&eevents[i]);
        evt->setEventLoop(this);
        int fd = eevents[i].data.fd;
        evt->setType(handleMap[fd]);
        std::shared_ptr<Event> ptr(evt);
        evts.push_back(ptr);
    }

    return evts;
}