#pragma once

#include <netinet/in.h>
#include <array>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

#define MESSAGE_BUFFER_LEN (100)

class Socket{
    private:
        int file_descriptor_;

    public:
        Socket();
        int getFd(){ return file_descriptor_;};
};

class InetSocketAddress{
    private:
        sockaddr_in address_;
    public:
        InetSocketAddress(uint32_t port); 
        InetSocketAddress(uint32_t port, std::string ip_address);
        void setIpAddress(std::string ip_address);
        sockaddr* getAddrPtr(){return (sockaddr*)&address_;};
        size_t getAddrSize(){return sizeof(address_);};
};

class UdpCommunicationHandler{
    private:
        Socket socket_;
        InetSocketAddress addr_listen_;
        InetSocketAddress addr_send_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        std::function<void(std::string, std::string)> receive_callback_;
        std::mutex mtx_;
        std::queue<std::string> send_msg_q_;
        std::queue<std::string> send_ip_q_;

    
    public:
        UdpCommunicationHandler(uint32_t port,
            std::function<void(std::string, std::string)> receive_callback);
        void send(std::string ip_addr, std::string message);
        void run();
};