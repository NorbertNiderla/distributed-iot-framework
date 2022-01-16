#include "include/udp.hpp"
#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>

UdpServer::UdpServer(){
    _fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    _addr_in = {.sin_family = AF_INET,
                .sin_port = htons(DEFAULT_COMMUNICATION_PORT),
                .sin_addr = {.s_addr = htonl(INADDR_LOOPBACK)}};
    memset(_recv_message_buffer, 0, RECV_MESSAGE_BUFFER_LEN);
    
    if(_fd == -1){
        perror("UdpServer::UdpServer : socket()");
        return;
    }
    
    if(bind(_fd, (sockaddr*)&_addr_in, sizeof(_addr_in)) == -1){
        perror("UdpServer::UdpServer : bind()");
        return;
    }
}

void UdpServer::run(){
    while(true){
        int recv_return = recv(_fd, (void*)_recv_message_buffer,
                               RECV_MESSAGE_BUFFER_LEN,
                               MSG_WAITALL);

        if(recv_return == -1){
            perror("UdpServer::run : recv()");
            return;
        }

        std::string temp_str = std::string((char*)_recv_message_buffer, recv_return);
        std::cout << temp_str << std::endl;
    }
}

UdpClient::UdpClient(){
    _fd_dst = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    if(_fd_dst == -1){
        perror("UdpClient::UdpClient() : socket()");
        return;
    }

    _addr_out = {.sin_family = AF_INET,
                               .sin_port = htons(DEFAULT_COMMUNICATION_PORT),
                               .sin_addr = {.s_addr = htonl(INADDR_LOOPBACK)}};
}

void UdpClient::run(){
    while(true){
        std::cin >> _input;
        int send_return = sendto(_fd_dst, (void*)_input.data(), _input.length(),
                                 0, (sockaddr*)&_addr_out, sizeof(_addr_out));
        
        if(send_return == -1){
            perror("UdpClient::run() : sendto()");
            return;
        }
            
    }
}