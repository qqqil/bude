#include "epoll_event.h"

void* EpollEvent::getData(){
    return eevent;
}

EpollEvent::~EpollEvent(){
    logger.log("deconstrctor");
    // if(eevent != nullptr){
    //     delete eevent;
    //     if(type == 1){
    //         int fd = eevent->data.fd;
    //         close(fd);
    //     }
    // }
}
bool EpollEvent::isAccept(){
    return type & 0x01<<16;//accept type is 0x01 ,others 0x10
}
bool EpollEvent::isRead(){
    // logger.log("events:");
    // logger.log(eevent->events);

    // logger.log("EPOLLIN:");
    // logger.log(EPOLLIN);
    // logger.log("EPOLLOUT:");
    // logger.log(EPOLLOUT);
    return !isAccept() && (eevent->events & EPOLLIN);
}
bool EpollEvent::isWrite(){
    return eevent->events & EPOLLOUT;
}
void EpollEvent::setEventLoop(EventLoop* evtloop){
    eventLoop = evtloop;
}
void EpollEvent::setType(int evtType){
    type = evtType;
}

int EpollEvent::getType(){
    return type;
}
int EpollEvent::getFd(){
    return eevent->data.fd;
}
EventLoop* EpollEvent::getEventLoop(){
    return eventLoop;
}