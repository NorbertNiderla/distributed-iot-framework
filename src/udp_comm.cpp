#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>

#include "include/udp_comm.hpp"
#include "include/logger.hpp"

Socket::Socket(){
    file_descriptor_ = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
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
        : socket_(), addr_listen_(port), addr_send_(port),
            receive_callback_(receive_callback){
}

void UdpCommunicationHandler::send(std::string ip_addr, std::string message){
    mtx_.lock();
    string_to_send_ = message;
    addr_send_.setIpAddress(ip_addr);
    something_to_send_flag_ = 1;
    mtx_.unlock();

    //this way does not prevent from override some messeges if there is a lot 
    //  of messages to send and receive in one moment
    // some kind of queue should be implemented
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
    
    int bind_ret = bind(socket_.getFd(), addr_listen_.getAddrPtr(), 
        addr_listen_.getAddrSize());
    
    if(bind_ret == -1){
        throw std::runtime_error("Bind failed");
    }

    _setNonBlocking(socket_.getFd());

    sockaddr_in incoming_addr;
    socklen_t incoming_addr_size = sizeof(incoming_addr);

    while(true){
        int recv_ret = recvfrom(socket_.getFd(), buffer_.data(),
            buffer_.max_size(),MSG_WAITALL, (sockaddr*)&incoming_addr,
            &incoming_addr_size);

        if(recv_ret != -1){ 
            //received something
            _LOG(DEBUG) << "received something";
            char ip_address_ch[INET_ADDRSTRLEN];
            const char* conv_ret = inet_ntop(AF_INET,
                &incoming_addr.sin_addr.s_addr,ip_address_ch, INET_ADDRSTRLEN);
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
        if(something_to_send_flag_ == 1){
            mtx_.lock();
            int send_ret = sendto(socket_.getFd(),
                (void*)string_to_send_.data(), string_to_send_.length(), 0,
                addr_send_.getAddrPtr(), addr_send_.getAddrSize());

            something_to_send_flag_ = 0;
            mtx_.unlock();

            if(send_ret == -1){
                throw(std::runtime_error("Sending failed"));
            }
    
            _LOG(DEBUG) << "message sent";
        }
    }
}
