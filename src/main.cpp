#include <string>
#include <iostream>
#include "include/udp_client.hpp"
#include "include/logger.hpp"

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

int main(int argc, char* argv[]){
    
    logger.set_logging_level(DEBUG);

    if(_check_input_arguments(argc, argv))
        return 1;

    _LOG(INFO) << "starting app";
    
    try{
        if(!std::string("receive").compare(std::string(argv[1]))){
            UdpClient client(INET_PORT_LISTEN);
            client.receive_run();
        } else if(!std::string("send").compare(std::string(argv[1]))) {
            UdpClient client(INET_PORT_LISTEN);
            std::string message = "message to other client";
            client.send(std::string("192.168.0.2"), message);
        } else {
            _LOG(ERROR) << "Unknown option\n";
        }
    }    
    catch(const std::exception &ex){
        _LOG(WARNING) << ex.what();
    }
    
    return 0;
}