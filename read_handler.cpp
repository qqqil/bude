#include "readHandler.h"
#include "epoll_event.h"
#include "sockets.h"

bool ReadHandler::isSupport(Event& evt){
    EpollEvent& eref = static_cast<EpollEvent&>(evt);
    return eref.isRead();
}

void ReadHandler::handle(Event& evt){
    EpollEvent& eref = static_cast<EpollEvent&>(evt);

    logger.log("do read handle for fd: ");

    int fd = eref.getFd();

    logger.log(fd);
    
    if(handleTable.find(fd) == handleTable.end()){
        handleTable[fd] = Buffer();
    }
    
    Buffer& buffer =handleTable[fd];
    char* buf = buffer.getBuf();
    size_t num = Sockets::read(fd,buf,buffer.getCap(),0);

    if(num == -1){
        logger.log("read data from connection error");
    }
    if(num >0){
        char logBuf[4096];
        int len=0;
        buffer.getData(logBuf,len);

        logger.log(std::string(logBuf));
    }
}