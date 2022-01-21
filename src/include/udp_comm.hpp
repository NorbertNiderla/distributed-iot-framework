#pragma once

#include <netinet/in.h>
#include <array>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>

#define MESSAGE_BUFFER_LEN (100)

class Socket{
    private:
        int file_descriptor_;

    public:
        Socket();
        int getFd(){ return file_descriptor_};
};

template <typename sockaddr_type>
class InetSocketAddress{
    private:
        sockaddr_type address_;
    public:
        InetSocketAddress(uint32_t port); 
        InetSocketAddress(uint32_t port, std::string ip_address);
        void setIpAddress(std::string ip_address);
        sockaddr* getAddrPtr(){return (sockaddr*)&address_};
        size_t getAddrSize(){return sizeof address_};
};

typedef std::shared_ptr<std::string> string_ptr_t;

class UdpCommunicationHandler{
    private:
        Socket socket_;
        InetSocketAddress<sockaddr_in> addr_listen_;
        InetSocketAddress<sockaddr_in> addr_send_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        string_ptr_t string_to_send_;
        int something_to_send_flag_ = 0;
        std::function<void(std::string, std::string)> receive_callback_;
        std::mutex mtx_;

    
    public:
        template <typename Func>
            UdpCommunicationHandler(uint32_t port,
                std::function<void(std::string, std::string)> receive_callback);
        void send(std::string ip_addr, string_ptr_t message);
        void run();
};