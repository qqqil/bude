#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <string>
class Logger{

    public:
    Logger ():name(""){

    }
    Logger (std::string tag):name(tag){

    }
    template<typename T>
     void log(T msg){
         std::cout<<name+" ";
         std::cout<<msg;
         std::cout<<std::endl;
     }
     private:
     std::string name;
};

#endif /* LOGGER_H */