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
#include "readHandler.h"
#include "write_handler.h"


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
    eevt.setType(0x01<<16);//accept event

    eevt.setEventLoop(&evtLoop);

    evtLoop.addEvent(eevt);

    AcceptHandler acceptHandler;
    reactor.registerHandler(acceptHandler);

    ReadHandler readHandler;

    reactor.registerHandler(readHandler);

    WriteHandler writeHandler;
    reactor.registerHandler(writeHandler);

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