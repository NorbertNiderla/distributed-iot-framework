#pragma once

#include <netinet/in.h>
#include <array>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

#define MESSAGE_BUFFER_LEN (100)
#define PROTOCOL_UDP  (0)
#define PROTOCOL_TCP  (1)

class Socket{
    private:
        int file_descriptor_;
    public:
        Socket(int type);
        int getFd(){ return file_descriptor_;};
};

class InetSocketAddress{
    private:
        sockaddr_in address_;
    public:
        InetSocketAddress();
        InetSocketAddress(uint32_t port); 
        InetSocketAddress(uint32_t port, std::string ip_address);
        void setIpAddress(std::string ip_address);
        sockaddr* getAddrPtr(){return (sockaddr*)&address_;};
        void* getSAddrPtr(){return &address_.sin_addr.s_addr;};
        socklen_t getAddrSize(){return (socklen_t)sizeof(address_);};
};

class SendingQueue{
    private:
        std::queue<std::string> send_msg_q_;
        std::queue<std::string> send_ip_q_;
        std::mutex mtx_;

    public:
        int getToSend(std::string &ip, std::string &msg);
        void pushToSend(std::string ip, std::string msg);
};

class UdpCommunicationHandler{
    private:
        int port_;
        Socket socket_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        std::function<void(std::string, std::string)> receive_callback_;
        SendingQueue sending_queue_;

    public:
        UdpCommunicationHandler(uint32_t port,
            std::function<void(std::string, std::string)> receive_callback);
        void send(std::string ip_addr, std::string message);
        void run();
};
/*
#define MAX_TCP_CONNECTIONS (5)

class TcpCommunicationHandler{
    private:
        int port_;
        Socket socket_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        std::function<void(std::string, std::string)> accept_callback_;
        std::thread thd_;
        std::vector<TcpConnectionHandler> conn_;
        void addConnection(int fd);
        void checkConnections();
    public:
        TcpCommunicationHandler(uint32_t port,
            std::function<void(std::string, std::string)> accept_callback);
        void run();
};

class TcpConnectionHandler{
    private:
        int fd_;
        SendingQueue queue;
    public:
        TcpConnectionHandler(int fd):fd_(fd){};
        void send(std::string ip_addr, std::string message);
};
*/
