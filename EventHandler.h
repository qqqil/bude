//
// Created by qqqil on 16-5-13.
//

#ifndef POLLTEST_EVENTHANDLER_H
#define POLLTEST_EVENTHANDLER_H

enum Event_Type{
    SOCK=0,SIGNAL=1
};

class EventHandler {
public:
    virtual  int get_type();
};


#endif //POLLTEST_EVENTHANDLER_H
