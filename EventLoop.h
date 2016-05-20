//
// Created by qqqil on 16-5-13.
//

#ifndef POLLTEST_EVENTLOOP_H
#define POLLTEST_EVENTLOOP_H


#include <queue>
#include <map>
#include <memory>
#include "EventHandler.h"
#include "Event.h"
#include "Logger.h"

class EventLoop {
public:
    EventLoop(Event& evt):epoll_evt(evt),logger("EventLoop"){
        _efd = epoll_create1(0);
        if(_efd == -1){
            logger.error("create epoll file description error!");
            exit(-1);
        }
        add_event(&evt);
    }
    ~EventLoop(){
        close(_efd);
    }
    void register_handler(EventHandler &handler);
    void remove_handler(EventHandler &handler);
    void loop();
    void add_event(Event* evt);
    void remove_event(Event& evt);
    int make_socket_non_blocking (int sfd);
    Event& get_event_by_fd(int fd);
private:
    typedef std::deque<std::shared_ptr<EventHandler>> Queue;
    typedef std::map<uint32_t ,std::shared_ptr<Event>> events_type;
    Queue handlers;
    events_type events;
    Event& epoll_evt;
    Logger logger;
    int _efd;
    struct epoll_event epoll_events[1024];
};


#endif //POLLTEST_EVENTLOOP_H
