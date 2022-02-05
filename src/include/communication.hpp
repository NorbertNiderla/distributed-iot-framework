#pragma once

#include <netinet/in.h>
#include <array>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>

#include "queue.hpp"

#define MESSAGE_BUFFER_LEN (100)
#define PROTOCOL_UDP  (0)
#define PROTOCOL_TCP  (1)

class MessageQueue{
    private:
        std::queue<std::string> send_msg_q_;
        std::queue<std::string> send_ip_q_;
        std::mutex mtx_;

    public:
        int pop(std::string &ip, std::string &msg);
        void push(std::string ip, std::string msg);
};

typedef struct{
    std::string msg;
    std::string ip;
}udp_message_t;

class UdpCommunicationHandler{
    private:
        int port_;
        int fd_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        std::function<void(std::string, std::string)> receive_callback_;
        QueueThreadSafe<udp_message_t> sending_queue_;
        std::atomic<bool> stop_; 

    public:
        UdpCommunicationHandler(uint32_t port,
            std::function<void(std::string, std::string)> receive_callback);
        void send(std::string ip_addr, std::string message);
        void run();
        int getPort() const {return port_;};
        int getFd() const {return fd_;};
        int checkSendingQueue(udp_message_t &message){
            return sending_queue_.pop(message);
        }
        void stopRunning() {stop_ = true;};
        
};
