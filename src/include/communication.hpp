#pragma once

#include <netinet/in.h>
#include <array>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>

#include "queue.hpp"
#include "ip_v4.hpp"

#define MESSAGE_BUFFER_LEN (100)
#define PROTOCOL_UDP  (0)
#define PROTOCOL_TCP  (1)

typedef struct{
    std::string msg;
    IPv4 ip;
}udp_message_t;

class UdpCommunicationHandler{
    private:
        int port_;
        int fd_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        std::function<void(IPv4, std::string)> receive_callback_;
        QueueThreadSafe<udp_message_t> sending_queue_;
        std::atomic<bool> stop_; 

    public:
        UdpCommunicationHandler(uint32_t port,
            std::function<void(IPv4, std::string)> receive_callback);
        void send(IPv4 ip, std::string message);
        void run();
        int getPort() const {return port_;};
        int getFd() const {return fd_;};
        int checkSendingQueue(udp_message_t &message){
            return sending_queue_.pop(message);
        }
        void stopRunning() {stop_ = true;};
        
};
