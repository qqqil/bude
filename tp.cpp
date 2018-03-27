#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <memory>

#include "logger.h"

Logger logger;
void t_f(){

    logger.log("thread work");
}

//thread :
// create thread example:
/*
    std::thread t(fun);




*/
//future : the class template future iprovides a mechanism to access the result of asynchronous operations 
//* packaged_task make the callable target(function,lambda,bind expression,function object)  async 
//  


// lock 
// lock_guard  unique_lock 
// 
// template <class F,class... Args>

class ThreadPool{

public:
    ThreadPool(int thread_size)
        :t_size(thread_size),stop(false)
    {
        for(int i=0;i<t_size;i++){
            workers.emplace_back(std::thread([this](){
                for(;;){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lk(this->mtx);
                        this->cond.wait(lk,[this]{return this->stop || !this->tasks.empty();});
                        
                        if(this->stop && this->tasks.empty()){
                            return;
                        }
                        // logger.log("do work..");
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            }));
    }
    }
    ~ThreadPool(){
        _logger.log("destory thread pool");
        {
            std::unique_lock<std::mutex>(mtx);
            stop = true;
        }
        cond.notify_all();
        for(std::thread& worker: workers){
            worker.join();
        }
    }
    template <class F,class... Args>
    auto enqueue(F&& fun,Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type> 
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(fun),std::forward<Args...>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lk(mtx);
            tasks.push([task]{(*task)();});
        }
        
        cond.notify_one();

        return res;
    }
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()> > tasks;
    
    std::mutex mtx;
    std::condition_variable cond;
    int t_size;
    Logger _logger;
    bool stop;
};

int main(){
    ThreadPool testpool(20);

    for(int i=0;i<1000000;i++){
        testpool.enqueue([](int num){;},i);
    }
    
    return 0;
}