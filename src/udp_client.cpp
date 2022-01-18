#include "include/udp_client.hpp"
#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>

UdpClient::UdpClient(){
    _fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    if(_fd == -1){
        throw(std::exception());
    }

    _addr = {.sin_family = AF_INET, .sin_port = htons(DEFAULT_COMMUNICATION_PORT),
        .sin_addr = {.s_addr = htonl(INADDR_LOOPBACK)}};
}

void UdpClient::send(){
    std::string message =  std::string("message to other udp client");
    int send_return = sendto(_fd, (void*)message.data(), message.length(),
                                 0, (sockaddr*)&_addr, sizeof(_addr));

    if(send_return == -1){
        throw(std::exception());
    }
}

void UdpClient::receive_run(){
    if(bind(_fd, (sockaddr*)&_addr, sizeof(_addr)) == -1){
        throw std::exception();
    }

    while(true){
        int recv_return = recv(_fd, _buffer.data(), _buffer.max_size(),
            MSG_WAITALL);

        if(recv_return == -1){ 
            throw std::exception();
        }

        printf("%s\n", _buffer.data());
    }
}
