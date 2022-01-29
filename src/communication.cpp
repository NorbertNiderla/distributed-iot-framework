#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>

#include "include/communication.hpp"
#include "include/logger.hpp"

Socket::Socket(int type){
    if(type == P_UDP)
        file_descriptor_ = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    else if(type == P_TCP)
        file_descriptor_ = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
    
    if(file_descriptor_ == -1){
        throw(std::runtime_error("Opening socket failed"));
    }
}

InetSocketAddress::InetSocketAddress(uint32_t port){
    // sets IpAdress to INADDR_ANY
    address_ = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
}

void InetSocketAddress::setIpAddress(std::string ip_address){
    int ret = inet_pton(AF_INET, ip_address.c_str(), &address_.sin_addr.s_addr);
    //inet_pton() already converts address into net byte order
    if(ret != 1){
        throw(std::invalid_argument("Invalid IpAddress"));
    }
}

InetSocketAddress::InetSocketAddress(uint32_t port, std::string ip_address){
    address_ = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
    setIpAddress(ip_address);
}

UdpCommunicationHandler::UdpCommunicationHandler(uint32_t port,
    std::function<void(std::string, std::string)> receive_callback)
        : socket_(P_UDP), receive_callback_(receive_callback), port_(port){}

void UdpCommunicationHandler::send(std::string ip_addr, std::string message){
    mtx_.lock();
    send_msg_q_.push(message);
    send_ip_q_.push(ip_addr);
    mtx_.unlock();
}

static void _setNonBlocking(int fd)
{
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if(ret == -1){
        throw(std::runtime_error("Could not set socket to nonblocking"));
    }
}     

void UdpCommunicationHandler::run(){

    InetSocketAddress listen_addr(port_);
    int bind_ret = bind(socket_.getFd(), listen_addr.getAddrPtr(), 
        listen_addr.getAddrSize());
    
    if(bind_ret == -1)
        throw std::runtime_error("Bind failed");

    _setNonBlocking(socket_.getFd());

    socklen_t incoming_addr_size = sizeof(listen_addr);

    InetSocketAddress send_addr(port_);

    while(true){
        int recv_ret = recvfrom(socket_.getFd(), buffer_.data(),
            buffer_.max_size(),MSG_WAITALL, listen_addr.getAddrPtr(),
            &incoming_addr_size);

        if(recv_ret != -1){ 
            //received something
            _LOG(DEBUG) << "received something";
            
            char ip_address_ch[INET_ADDRSTRLEN];
            
            const char* conv_ret = inet_ntop(AF_INET,
                listen_addr.getSAddrPtr(),ip_address_ch, INET_ADDRSTRLEN);
            
            if(conv_ret == nullptr){
                throw(std::runtime_error("Conversion of IpAddress to string interpretation failed"));
            }
            //here should be a way to restart UdpHandler::run(),
            //  maybe run() should be in wrapper to catch all exceptions?

            _LOG(INFO) << std::string("received: ") + std::string(buffer_.data());

            receive_callback_(std::string(ip_address_ch), 
                std::string(buffer_.data()));   
        }
        //here should be done error checking for recvfrom

        //check for any messages to send
        if(send_msg_q_.size() != 0){
            mtx_.lock();
            std::string msg = send_msg_q_.front();
            std::string ip = send_ip_q_.front();
            send_msg_q_.pop();
            send_ip_q_.pop();
            mtx_.unlock();


            send_addr.setIpAddress(ip);

            int send_ret = sendto(socket_.getFd(),
                (void*)msg.data(), msg.length(), 0,
                send_addr.getAddrPtr(), send_addr.getAddrSize());

            if(send_ret == -1){
                throw(std::runtime_error("Sending failed"));
            }
    
            _LOG(DEBUG) << "message sent";
        }
    }
}
