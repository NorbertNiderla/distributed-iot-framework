#include "include/udp_client.hpp"
#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

UdpClient::UdpClient(uint32_t port){
    _fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    if(_fd == -1){
        throw(std::exception());
    }

    _addr = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
}

void UdpClient::send(){
    std::chrono::seconds timespan(2);

    std::string message =  std::string("some other message");    
    _addr.sin_addr.s_addr = htonl(0xC0A80002);
    
    int i = 0;
    while(i++ < 20){
        int send_return = sendto(_fd, (void*)message.data(), message.length(),
            0, (sockaddr*)&_addr, sizeof(_addr));
        
        if(send_return == -1){
            throw(std::exception());
        }

        std::cout << "message sent\n";
        std::this_thread::sleep_for(timespan);
    }
}

void UdpClient::receive_run(){
    
    uint8_t ret = bind(_fd, (sockaddr*)&_addr, sizeof(_addr));
    
    if(ret == -1){
        throw std::exception();
    }

    int i = 0;
    while(i++ < 20){
        std::cout << "trying to receive\n";
            int recv_return = recv(_fd, _buffer.data(), _buffer.max_size(),
            MSG_WAITALL);

        if(recv_return == -1){ 
            throw std::exception();
        }
        
        printf("%s\n", _buffer.data());
    }

}
