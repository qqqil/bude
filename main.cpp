#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <zconf.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "EpollEvent.h"
#include "EventLoop.h"

using namespace std;

#define LOG cout
#define END endl

Logger logger("main");

static int
make_socket_non_blocking (int sfd)
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

int create_sock(struct sockaddr_in& addr,bool is_block){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1){
        logger.error(strerror(errno));
        exit(-1);
    }
    int ret = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    if(ret == -1){
        logger.error(strerror(errno));
        exit(-1);
    }
    if(!is_block){
        make_socket_non_blocking(sock);
    }
    ret  = listen(sock,16);
    if(ret == -1){
        logger.error(strerror(errno));
        exit(-1);
    }
    return sock;
}
void start_poll(){
    logger.info("poll test");
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;
    int server_sock = create_sock(addr,true);

    struct pollfd fds[100];
    fds[0].fd = server_sock;
    fds[0].events = fds[0].events | POLLIN |POLLOUT;
    bool is_end = true;
    int numfds =0;
    numfds++;

    int count =0;
    while(is_end){
        int ret = poll(fds,numfds,5*1000);
        count++;
        if(count == 1000){
            is_end = false;
        }
        if(ret == 0){
            logger.error("TIMEOUT");
            continue;
        }
        for(int i=0;i<numfds;i++){
            if(fds[i].revents & POLLIN){

                logger.info("has events for read:");
                if(fds[i].fd == server_sock){
                    LOG<<"accept for client"<<END;
                    int client = accept(server_sock,NULL,NULL);
                    fds[numfds].fd=client;
                    fds[numfds].events = POLLIN | POLLOUT;
                    numfds++;
                }else{
                    char buf[1024];
                    bzero(buf,sizeof(buf));
                    ssize_t  num = read(fds[i].fd,buf,1024);
                    string str(buf);
                    LOG<<"RECV from:"<<fds[i].fd<<"-"<<str<<END;
                }
            }
            if(fds[i].revents & POLLOUT){
                LOG<<"has events for write:"<<i<<END;
                char* out = "this is a message from server";
                write(fds[i].fd,out,strlen(out));
                close(fds[i].fd);
                for(int k=i;k<numfds;k++){
                    fds[k].fd = fds[k+1].fd;
                }
                numfds--;
            }
            if(fds[i].revents & POLLHUP){
                LOG<<"close fd "<<i<<END;
                close(fds[i].fd);
            }
        }
    }
    close(server_sock);
}

void epoll_test(){
    LOG<<"start test.."<<END;
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;
    int server_sock = create_sock(addr,false);

    int efd = epoll_create1(0);
    if(efd == -1){
        LOG<<strerror(errno)<<END;
        exit(-1);
    }
    struct epoll_event event;
    struct epoll_event events[256];

    event.data.fd = server_sock;
    event.events  =EPOLLIN | EPOLLET;

    EpollEvent new_event(server_sock);
    EventLoop evt_loop(new_event);

    int ret = epoll_ctl(efd,EPOLL_CTL_ADD,server_sock,&event);
    if(ret == -1){
        LOG<<strerror(errno)<<END;
        exit(-1);
    }
    bool is_end = true;
    int count =0;
    while(is_end){
        int event_num = epoll_wait(efd,events,256,-1);
        count++;
        if(event_num == -1){
            LOG<<strerror(errno)<<END;
            exit(-1);
        }
        if(count == 1000){
            is_end = false;
        }
        if(event_num == 0){
            LOG<<"TIMEOUT"<<END;
            continue;
        }
        for(int i=0;i<event_num;i++){
            if(events[i].events & EPOLLIN){

                LOG<<"has events for read:"<<i<<END;
                if(events[i].data.fd == server_sock){
                    LOG<<"accept for client"<<END;
                    int client = accept(server_sock,NULL,NULL);
                    event.data.fd=client;
                    event.events = EPOLLIN | EPOLLOUT|EPOLLET;
                    make_socket_non_blocking(client);
                    epoll_ctl(efd,EPOLL_CTL_ADD,client,&event);
                }else{
                    char buf[1024];
                    bzero(buf,sizeof(buf));
                    ssize_t  num = read(events[i].data.fd,buf,1024);
                    string str(buf);
                    LOG<<"RECV from:"<<events[i].data.fd<<"-"<<str<<END;
                }
            }
            if(events[i].events & EPOLLOUT){
                LOG<<"has events for write:"<<i<<END;
                char* out = "this is a message from server";
                write(events[i].data.fd,out,strlen(out));
                close(events[i].data.fd);

            }
            if(events[i].events & POLLHUP){
                LOG<<"close fd "<<i<<END;
                close(events[i].data.fd);
            }
        }
    }
    close(server_sock);
}
int main() {
    LOG<<"start test.."<<END;
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;
    int server_sock = create_sock(addr,false);

    int efd = epoll_create1(0);
    if(efd == -1){
        LOG<<strerror(errno)<<END;
        exit(-1);
    }
    struct epoll_event event;
    struct epoll_event events[256];

    event.data.fd = server_sock;
    event.events  =EPOLLIN | EPOLLET;

    EpollEvent new_event(server_sock);
    EventLoop evt_loop(new_event);

//    int ret = epoll_ctl(efd,EPOLL_CTL_ADD,server_sock,&event);
//    if(ret == -1){
//        LOG<<strerror(errno)<<END;
//        exit(-1);
//    }
    bool is_end = true;
    int count =0;
    evt_loop.loop();
    close(server_sock);
    return 0;
}