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

class MessageQueue{
    private:
        std::queue<std::string> send_msg_q_;
        std::queue<std::string> send_ip_q_;
        std::mutex mtx_;

    public:
        int pop(std::string &ip, std::string &msg);
        void push(std::string ip, std::string msg);
};

class UdpCommunicationHandler{
    private:
        int port_;
        int fd_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
        std::function<void(std::string, std::string)> receive_callback_;
        MessageQueue sending_queue_;

    public:
        UdpCommunicationHandler(uint32_t port,
            std::function<void(std::string, std::string)> receive_callback);
        void send(std::string ip_addr, std::string message);
        void run();
        int getPort() const {return port_;};
        int getFd() const {return fd_;};
        int checkSendingQueue(std::string &ip, std::string &msg){
            return sending_queue_.pop(ip, msg);
        }
        
};

#define MAX_TCP_CONNECTIONS (5)

class TcpCommunicationHandler{
    private:
        int port_;
        Socket socket_;
        std::function<void(std::string, std::string)> receive_callback_;
        std::thread thd_;
        std::vector<TcpConnectionHandler> connections_;
        void addConnection(int fd, std::string ip_addr);
        void checkConnections();
        void sendAcceptResponse(int fd, std::string ip_addr);
    public:
        TcpCommunicationHandler(uint32_t port,
            std::function<void(std::string, std::string)> receive_callback_);
        void run();
};

class TcpConnectionHandler{
    private:
        int fd_;
        SendingQueue queue_;
        InetSocketAddress addr_;
        std::array<char, MESSAGE_BUFFER_LEN> buffer_ = {};
    public:
        TcpConnectionHandler(int fd):fd_(fd){};
        void send(std::string ip_addr, std::string message);
        int getFd(){return fd_;};
        char* getBufferData(){return buffer_.data();};
        int getBufferMaxSize(){return MESSAGE_BUFFER_LEN;};
        int getToSend(std::string ip, std::string msg){return queue_.getToSend(ip, msg);};

};
