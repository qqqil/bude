//
// Created by qqqil on 16-5-13.
//

#ifndef POLLTEST_EVENT_H
#define POLLTEST_EVENT_H

#include <sys/epoll.h>
#include <zconf.h>

class Event {
public:
    typedef struct epoll_event event_data;
    Event(int _fd){
        data.data.fd=_fd;
        data.events=EPOLLIN|EPOLLOUT|EPOLLET;
    }
    virtual ~Event(){
        
    }
    virtual  event_data& get_event() =0;
    virtual int get_event_type() =0;
    void set_readable(bool val);
    void set_writeable(bool val);
    void set_complete(bool val);
    void set_error(bool val);
protected:
    event_data data;
    char buffer[8096];
public:
    bool is_read() const {
        return _read;
    }

    void set_read(bool _read) {
        Event::_read = _read;
    }

    bool is_write() const {
        return _write;
    }

    void set_write(bool _write) {
        Event::_write = _write;
    }


    bool is_complete() const {
        return _complete;
    }

    bool is_error() const {
        return _error;
    }

protected:
    bool _read;
    bool _write;
    bool _complete;
    bool _error;
};


#endif //POLLTEST_EVENT_H
