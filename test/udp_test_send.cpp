#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <netinet/in.h>
#include <array>
#include "./../src/include/logger.hpp"

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
        .sin_addr = {.s_addr = htonl(INADDR_LOOPBACK)}};
}

void UdpClient::send(){
    std::chrono::seconds timespan(2);

    std::string message =  std::string("some other message");    
    //_addr.sin_addr.s_addr = htonl(0xC0A80002);
    
    int i = 0;
    while(i++ < 2){
        int send_return = sendto(_fd, (void*)message.data(), message.length(),
            0, (sockaddr*)&_addr, sizeof(_addr));
        
        if(send_return == -1){
            throw(std::exception());
        }

        std::cout << "message sent\n";
        std::this_thread::sleep_for(timespan);
    }
}

#define INET_PORT_LISTEN  (6300)
#define INET_PORT_SEND    (6301)

bool static _check_input_arguments(int argc, char* argv[]){
    if(argc != 2){ 
        _LOG(ERROR) << "Number of additional arguments is not 1";
        std::cout << "Input arguments:";
        for(int i = 1; i < argc; i++)
            std::cout << argv[i] << " ";
        std::cout << "\n";
        return 1;
    }
    return 0;
}

int main(){
    
    logger.set_logging_level(DEBUG);


    _LOG(INFO) << "starting app";
    try{
        UdpClient client(INET_PORT_LISTEN);
        std::string message = "message to other client";
        client.send();
    }     
    catch(const std::exception &ex){
        _LOG(WARNING) << ex.what();
    }
    
    return 0;
}