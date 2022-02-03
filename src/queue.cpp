#include "include/queue.hpp"

template<typename Element>
int QueueThreadSafe<Element>::pop(Element &obj){
    mtx_.lock();
    int ret = queue_.size();
    if(ret != 0){
        obj = queue_.front();
        queue_.pop();
    }
    mtx_.unlock();
    return ret;
}

template<typename Element>
void QueueThreadSafe<Element>::push(Element &obj){
    mtx_.lock();
    queue_.push(obj);
    mtx_.unlock();
}