#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>

#include "include/communication.hpp"
#include "include/logger.hpp"

static int socketCreate(int type){
    int fd;
    if(type == PROTOCOL_UDP)
        fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    else if(type == PROTOCOL_TCP)
        fd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
    
    if(fd == -1){
        throw(std::runtime_error("Opening socket failed"));
    }
    return fd;
}

static sockaddr_in socketAddressCreate(int port){
    sockaddr_in address = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
    return address;
}

static void socketAddressSetIp(std::string ip_address, sockaddr_in* address){
    int ret = inet_pton(AF_INET, ip_address.c_str(), &(address->sin_addr.s_addr));
    //inet_pton() already converts address into net byte order
    if(ret != 1){
        //error checking should be more wide here
        throw(std::invalid_argument("Invalid IpAddress"));
    }
}

static sockaddr_in socketAddressCreate(int port, std::string ip_address){
    sockaddr_in addr = socketAddressCreate(port);
    socketAddressSetIp(ip_address, &addr);
    return addr;
}

static void socketSetNonBlocking(int fd)
{
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if(ret == -1){
        throw(std::runtime_error("Could not set socket to nonblocking"));
    }
}  

/* 
    recvfrom wrapper for receiving on socket
    modify ip and buffer on succesful receive
    TODO: checking recv ret error types
    when received something return 1, otherwise return 0
*/
int receiveFrom(sockaddr_in &listen_addr, int fd, void* buffer, int buffer_size,
    std::string &ip){
    
    socklen_t incoming_addr_size = sizeof(listen_addr);
    int recv_ret = recvfrom(fd, buffer, buffer_size,MSG_WAITALL,
        (sockaddr*)&listen_addr, &incoming_addr_size);

    if(recv_ret != -1){ 
        _LOG(DEBUG) << "received something";

        char ip_address_ch[INET_ADDRSTRLEN];
        const char* conv_ret = inet_ntop(AF_INET, &listen_addr.sin_addr.s_addr,
            ip_address_ch, INET_ADDRSTRLEN);
            
        if(conv_ret == nullptr){
            throw(std::runtime_error("Conversion of IpAddress to string interpretation failed"));
        }

        ip = std::string(ip_address_ch);

        _LOG(INFO) << std::string("received: ") + ip + " " + std::string((char*)buffer);

        return 1;
    }

    return 0;
}

/*
    wrapper for sendto function
    checking handlers sending queue and send it to port and fd of handler
*/
template <typename CommunicationHandler>
void checkAndSend(CommunicationHandler &handler){
    std::string ip;
    std::string msg;
    if(handler.checkSendingQueue(ip, msg) != 0){
        sockaddr_in addr = socketAddressCreate(handler.getPort(), ip);

        int send_ret = sendto(handler.getFd(), (void*)msg.data(), msg.length(), 0,
            (sockaddr*)&addr, sizeof(addr));

        if(send_ret == -1){
            throw(std::runtime_error("Sending failed"));
        }
    
        _LOG(DEBUG) << std::string("message sent ") + ip + std::string(" ") + msg;
    }
}

UdpCommunicationHandler::UdpCommunicationHandler(uint32_t port,
    std::function<void(std::string, std::string)> receive_callback)
        : receive_callback_(receive_callback), port_(port),
            fd_(socketCreate(PROTOCOL_UDP)){}

void UdpCommunicationHandler::send(std::string ip_addr, std::string message){
    sending_queue_.push(ip_addr, message);
}

void UdpCommunicationHandler::run(){
    
    sockaddr_in listen_addr = socketAddressCreate(port_);
    
    int bind_ret = bind(fd_, (sockaddr*)&listen_addr, sizeof(listen_addr));
    
    if(bind_ret == -1)
        throw std::runtime_error("Bind failed");

    socketSetNonBlocking(fd_);

    std::string msg;
    std::string ip;
    while(true){
        if(receiveFrom(listen_addr, fd_, buffer_.data(), buffer_.max_size(),ip) == 1)
            receive_callback_(ip, std::string(buffer_.data()));   
        
        checkAndSend(*this);
    }
}

int MessageQueue::pop(std::string &ip, std::string &msg){
    mtx_.lock();
    int ret = send_msg_q_.size();
    if(ret != 0){
        ip = send_ip_q_.front();
        msg = send_msg_q_.front();
        send_ip_q_.pop();
        send_msg_q_.pop();
    }
    mtx_.unlock();
    return ret;
}

void MessageQueue::push(std::string ip, std::string msg){
    mtx_.lock();
    send_ip_q_.push(ip);
    send_msg_q_.push(msg);
    mtx_.unlock();
}