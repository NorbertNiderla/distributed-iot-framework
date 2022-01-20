#pragma once

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