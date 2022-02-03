#pragma once
#include <mutex>
#include <queue>

#include "communication.hpp"

template<typename Element>
class QueueThreadSafe{
    protected:
        std::queue<Element> queue_;
        std::mutex mtx_;

    public:
        QueueThreadSafe(){};
        
        int pop(Element &obj){
            mtx_.lock();
            int ret = queue_.size();
            if(ret != 0){
                obj = queue_.front();
                queue_.pop();
            }
            mtx_.unlock();
            return ret;
        };

        void push(Element &obj){
            mtx_.lock();
            queue_.push(obj);
            mtx_.unlock();
        };
};