cmake_minimum_required(VERSION 2.8)

project(polltest)
set (CMAKE_CXX_STANDARD 11)


add_executable(test test.cpp logger.h Decrease.cpp)


add_executable(tp tp.cpp logger.h)

add_executable(reactor main.cpp 
                reactor.h 
                reactor.cpp 
                logger.h 
                epoll_event.h 
                epoll_event.cpp 
                event.h 
                event_handler.h 
                accept_event_handler.h 
                accept_event_handler.cpp 
                event_loop.h 
                epoll_event_loop.h 
                epoll_event_loop.cpp 
                buffer.h 
                buffer.cpp 
                readHandler.h 
                read_handler.cpp 
                write_handler.h 
                write_handler.cpp 
                )

TARGET_LINK_LIBRARIES( tp
pthread
)