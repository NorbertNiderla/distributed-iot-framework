#include "./../src/include/logger.hpp"
#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <array>

#define MESSAGE_BUFFER_LEN (100)

class UdpClient{
    private:
        int _fd;
        sockaddr_in _addr;
        std::array<char, MESSAGE_BUFFER_LEN> _buffer = {};
    
    public:
        UdpClient(uint32_t port);
        void send();
        void receive_run();
};

UdpClient::UdpClient(uint32_t port){
    _fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    if(_fd == -1){
        throw(std::exception());
    }

    _addr = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
}

void UdpClient::send(){

    std::string message =  std::string("message to other udp client");    
    _addr.sin_addr.s_addr = htonl(0xC0A80002);
    int send_return = sendto(_fd, (void*)message.data(), message.length(),
        0, (sockaddr*)&_addr, sizeof(_addr));
    
    if(send_return == -1){
        throw(std::exception());
    }

    std::cout << "message sent\n";
}

void UdpClient::receive_run(){
    
    uint8_t ret = bind(_fd, (sockaddr*)&_addr, sizeof(_addr));
    
    if(ret == -1){
        throw std::exception();
    }

    int recv_return = recv(_fd, _buffer.data(), _buffer.max_size(),
        MSG_WAITALL);

    if(recv_return == -1){ 
        throw std::exception();
    }
    
    printf("%s\n", _buffer.data());
    
}
