
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

class EventLoop;

class Event{
    public:
     Event(){}
     virtual void* getData(){};
     virtual ~Event(){

     }
};
class EpollEvent:public Event{
    public:

    EpollEvent(struct epoll_event* evt):eevent(evt){
        logger = Logger("epoll event");
            type = 0;
        logger.log("ctor @");
        logger.log(this);
    }

    EpollEvent(const EpollEvent& evt){
        logger.log("copy tor@");
        logger.log(this);
    }
    virtual void* getData(){
        return eevent;
    }

    ~EpollEvent(){
        logger.log("deconstrctor");
        // if(eevent != nullptr){
        //     delete eevent;
        //     if(type == 1){
        //         int fd = eevent->data.fd;
        //         close(fd);
        //     }
        // }
    }
    void setEventLoop(EventLoop* evtloop){
        eventLoop = evtloop;
    }
    void setType(int evtType){
        type = evtType;
    }
    int getFd(){
        return eevent->data.fd;
    }
    EpollEvent* getEventLoop(){
        return static_cast<EpollEventLoop*>(eventLoop);
    }
    private:
        struct epoll_event *eevent;
        EventLoop* eventLoop;
        int type;
        Logger logger;
};

class EventHandler{
    public:
        EventHandler(){
            _logger = Logger("event handler");
        }
        virtual void handle(Event& evt){
            _logger.log("handle");
        };
    private:
    Logger _logger;
};

class AcceptHandler:public EventHandler{
    public:
        AcceptHandler(){
            logger = Logger("acceptHandler");
        }
        virtual void handle(Event& evt){
            logger.log("do accept handler");
            EpollEvent& eevt = static_cast<EpollEvent&>(evt);

            // struct epoll_event* eev = static_cast<struct epoll_event*>(eevt.getData());
            struct sockaddr addr;
            int addrlen;
            int client_fd = accept(eevt.getFd(),NULL,NULL);
            struct epoll_event ev;
            ev.events = EPOLLIN|EPOLLOUT;
            ev.data.fd = client_fd;
            EpollEvent *new_eevt = new EpollEvent(&ev);
            

            eevt.getEventLoop()->addEvent(new_eevt);
            logger.log(eevt.getFd());
        }
    private:
    Logger logger;

};
class EventLoop{
    public:
        EventLoop(){}
        virtual ~EventLoop() {} ;
        virtual std::vector<std::shared_ptr<Event> > loop()=0;
        virtual void addEvent(Event& evt)=0;
        virtual void removeEvent(Event& evt)=0;
};

class EpollEventLoop:public EventLoop{
    public: 
        EpollEventLoop(){
            epollfd = epoll_create(1);
            if(epollfd == -1){
                logger.log("create epoll fd error");
                logger.log(strerror(errno));
            }
            eevents = new epoll_event[MAX_EVENTS];
        }
        virtual ~EpollEventLoop(){
            
        }
    std::vector<std::shared_ptr<Event> > loop();
    void addEvent(Event& evt);
    void removeEvent(Event& evt);
    private:
        struct epoll_event* eevents;      
        const int MAX_EVENTS = 64;
        int epollfd;
        Logger logger;
};

void EpollEventLoop::addEvent(Event& evt){
    struct epoll_event * eevt =(struct epoll_event*)evt.getData();
    int fd=eevt->data.fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,eevt);
    logger.log("add event");
}

void EpollEventLoop::removeEvent(Event& evt){
    struct epoll_event * eevt = (struct epoll_event*)evt.getData();
    int fd=eevt->data.fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,eevt);
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
        std::shared_ptr<Event> ptr(evt);
        evts.push_back(ptr);
    }

    return evts;
}


class Reactor{
public:
        
        Reactor(EventLoop& evtLoop):eventLoop(evtLoop){
            logger = Logger("Reactor ");
        }
        void registerHandler(EventHandler& handler){
            handlers.push_back(std::shared_ptr<EventHandler>(&handler));
        };
        void unregisterHandler(EventHandler& handler);
        void handleEvents();
private:
    EventLoop& eventLoop;
    std::vector<std::shared_ptr<EventHandler> > handlers;
    Logger logger;
};




void Reactor::handleEvents(){

    while(true){
        //get handler to handl event
        std::vector<std::shared_ptr<Event> > events = eventLoop.loop();
        logger.log("get events to handle ");
        logger.log(events.size());
        for(std::shared_ptr<Event> evt : events){
            for(std::shared_ptr<EventHandler> handler : handlers){
                logger.log("do handle");
                (*handler).handle(*evt);
            }
        }
        logger.log("handle events done! ");
    }
}


static int
make_socket_non_blocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        return -1;
    }

    return 0;
}

int create_sock(struct sockaddr_in& addr,bool is_block){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1){
        exit(-1);
    }
    int ret = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    if(ret == -1){
        exit(-1);
    }
    if(!is_block){
        make_socket_non_blocking(sock);
    }
    ret  = listen(sock,16);
    if(ret == -1){
        exit(-1);
    }
    return sock;
}


void test_reactor(){
    EpollEventLoop evtLoop;

    Reactor reactor(evtLoop);


    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;
    int server_sock = create_sock(addr,true);
    struct epoll_event *ev = new epoll_event();
    ev->data.fd = server_sock;
    ev->events = EPOLLIN;
    EpollEvent eevt(ev);
    eevt.setType(1);//accept event

    eevt.setEventLoop(&evtLoop);

    evtLoop.addEvent(eevt);

    AcceptHandler acceptHandler;
    reactor.registerHandler(acceptHandler);
    
    reactor.handleEvents();
}
int main(){
    Logger logger("main");
    test_reactor();
    // struct epoll_event *pt = nullptr;
    // EpollEvent *evt = new EpollEvent(pt);
    // std::shared_ptr<EpollEvent> p1(evt);
    // {
    //     std::vector<std::shared_ptr<EpollEvent> > vec;
    //     vec.push_back(p1);
    // }
    logger.log("completed!!");
    return 0;
}