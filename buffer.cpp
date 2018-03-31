
#include "buffer.h"
#include "string.h"

Buffer::Buffer(){
    logger= Logger("Buffer ");
    rstart = 0;
    rend = 0;
}

void Buffer::reset(){
    rstart = 0;
    rend = 0;
}
void Buffer::addData(char* data,int len){
    if(rend+len > 4096){
        logger.log("error data out of buf");
        return;
    }
    memcpy(buf+rend,data,len);
    rend +=len;
}
void Buffer::getData(char* data,int& len){
    data = buf;
    len = rend - rstart;

}

char* Buffer::getBuf(){
    return buf+rend;
}

int Buffer::getCap(){
    return 4096 - rend;
}