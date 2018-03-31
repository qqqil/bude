#ifndef __EVENT_H
#define __EVENT_H
class Event{
    public:
     Event(){}
     virtual void* getData(){};
     virtual ~Event(){

     }
     virtual int getType() = 0;
};

#endif
