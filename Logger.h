//
// Created by qqqil on 16-5-14.
//

#ifndef POLLTEST_LOGGER_H
#define POLLTEST_LOGGER_H

#include <iostream>

class Logger {
public:
    Logger(std::string name):_name(name),_error("ERROR"),
    _info("INFO")
    ,_debug("dEBUG"){

    }
    template <typename  T>
    void log(std::string level,T msg);
    template <typename  T>
    void info(T msg);
    template <typename  T>
    void debug(T msg);
    template <typename  T>
    void error(T msg);

private:
    std::string _name;
    std::string _error;
    std::string _info;
    std::string _debug;
};
template <typename T>
void Logger::log(std::string level,T msg) {
    std::cout<<_name<<"-";
    std::cout<<level<<":";
    std::cout<<msg;
    std::cout<<std::endl;
}

template <typename T>
void Logger::info(T msg) {
    log(_info,msg);
}
template <typename T>
void Logger::debug(T msg) {
    log(_debug,msg);
}
template <typename T>
void Logger::error(T msg) {
    log(_error,msg);
}
#endif //POLLTEST_LOGGER_H
