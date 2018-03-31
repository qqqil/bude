#ifndef __BUFFER_H
#define __BUFFER_H


#include "logger.h"

class Buffer{
    public:
    Buffer();
    void reset();
    void addData(char* data,int len);
    void getData(char* data,int& len);
    char* getBuf();
    int getCap();
    private:
        int rstart;
        int rend;
        int wstart;
        int wend;
        char buf[4096];
        Logger logger;
};

#endif
