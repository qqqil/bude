#ifndef __SOCKETS_H
#define __SOCKETS_H
#include <sys/types.h>
#include <sys/socket.h>


class Sockets{

    public:
     static ssize_t read(int fd,void* buf,size_t len,int flags){
         return recv(fd,buf,len,flags);
     } 
     static ssize_t write(int sockfd, const void *buf, size_t len, int flags){
         return send(sockfd,buf,len,flags);
     }
};

#endif