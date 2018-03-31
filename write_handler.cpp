#include "write_handler.h"
#include "epoll_event.h"
#include "sockets.h"
#include <string.h>

bool WriteHandler::isSupport(Event& evt){
    EpollEvent& eref = static_cast<EpollEvent&>(evt);
    return eref.isWrite();
}

void WriteHandler::handle(Event& evt){
    EpollEvent& eref = static_cast<EpollEvent&>(evt);

    logger.log("do write handle for fd: ");

    int fd = eref.getFd();

    logger.log(fd);
    
    if(handleTable.find(fd) == handleTable.end()){
        handleTable[fd] = Buffer();
    }
    
    Buffer& buffer =handleTable[fd];
    const char* buf = "say server";
    size_t num = Sockets::write(fd,buf,strlen(buf),0);

    if(num == -1){
        logger.log("write data to connection error");
    }
}