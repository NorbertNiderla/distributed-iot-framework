#pragma once
#include <mutex>
#include <queue>

template<typename Element>
class QueueThreadSafe{
    protected:
        std::queue<Element> queue_;
        std::mutex mtx_;

    public:
        QueueThreadSafe();
        int pop(Element &obj);
        void push(Element &obj);
};