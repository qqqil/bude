//
// Created by qqqil on 16-5-13.
//

#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <zconf.h>
#include <poll.h>
#include "EventLoop.h"
#include "EpollEvent.h"

void EventLoop::register_handler(EventHandler &handler) {
    std::shared_ptr<EventHandler> eptr;
    eptr.reset(&handler);
    handlers.push_back(eptr);
}

void EventLoop::remove_handler(EventHandler& handler) {
    auto itr = handlers.begin();
    for(; itr != handlers.end();++itr ){
        (*itr)->get_type() == handler.get_type();
        break;
    }
    if(itr != handlers.end()){
        handlers.erase(itr);
    }

}

int EventLoop::make_socket_non_blocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}
void EventLoop::loop() {
    while(true){
        int event_num = epoll_wait(_efd,epoll_events,1024,-1);
        if(event_num == -1){
            logger.error(strerror(errno));
            exit(-1);
        }

        if(event_num == 0){
            logger.error("TIMEOUT");
            continue;
        }

        for(int i=0;i<event_num;i++){
            if(epoll_events[i].events & EPOLLIN){
                logger.info("has events for read:");
                logger.info(i);
                if(epoll_events[i].data.fd == epoll_evt.get_event().data.fd){
                    logger.info("accept for client");
                    int client = accept(epoll_evt.get_event().data.fd,NULL,NULL);
                    struct epoll_event event;
                    event.data.fd=client;
                    event.events = EPOLLIN | EPOLLOUT|EPOLLET;
                    make_socket_non_blocking(client);
                    EpollEvent  *new_epoll_evt = new EpollEvent(client);
                    add_event(new_epoll_evt);
//                    epoll_ctl(epoll_evt.get_event().data.fd,EPOLL_CTL_ADD,client,&event);
                }else{
                    char buf[1024];
                    bzero(buf,sizeof(buf));
                    ssize_t  num = read(epoll_events[i].data.fd,buf,1024);
                    std::string str(buf);
                    logger.info("RECV from:");
                    logger.info(epoll_events[i].data.fd);
                    logger.info("-");
                    logger.info(str);
                }
            }
            if(epoll_events[i].events & EPOLLOUT){
                logger.info("has events for write:");
                logger.info(i);
                char* out = "this is a message from server";
                write(epoll_events[i].data.fd,out,strlen(out));
//                close(epoll_events[i].data.fd);
                remove_event(get_event_by_fd(epoll_events[i].data.fd));

            }
            if(epoll_events[i].events & POLLHUP){
                logger.info("close fd ");
                logger.info(i);
                remove_event(get_event_by_fd(epoll_events[i].data.fd));
            }
        }
    }

}

void EventLoop::add_event(Event *evt) {
    logger.info("add event ");
    make_socket_non_blocking(evt->get_event().data.fd);
    int fd = evt->get_event().data.fd;
    std::shared_ptr<Event> eptr;
    eptr.reset(evt);
    events.insert(std::make_pair(fd,eptr));
    struct epoll_event evt_data = evt->get_event();
    int ret = epoll_ctl(_efd,EPOLL_CTL_ADD,evt->get_event().data.fd,&evt_data);
    if(ret == -1){
        logger.error(strerror(errno));
        exit(-1);
    }
}

void EventLoop::remove_event(Event &evt) {
    logger.info("remove event");
    events.erase(evt.get_event().data.fd);
}

Event &EventLoop::get_event_by_fd(int fd) {
    return *events[fd];
}











