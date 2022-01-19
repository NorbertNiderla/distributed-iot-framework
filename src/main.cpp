#include <string>
#include <iostream>
#include "include/udp_client.hpp"

#define DEFAULT_COMMUNICATION_PORT  (6300)

#define _LOG(x) (std::cout << x << "\n");

int main(int argc, char* argv[]){
    
    _LOG("starting app")
    
    if(argc != 2){
        std::cout << "Number of options is not 2: " << argc << "\n";
        for(int i = 0; i < argc; i++)
            std::cout << argv[i] << "\n";
        return 1;
    }

    try{
        if(!std::string("receive").compare(std::string(argv[1]))){
            UdpClient client(DEFAULT_COMMUNICATION_PORT);
            client.receive_run();
        } else if(!std::string("send").compare(std::string(argv[1]))) {
            UdpClient client(DEFAULT_COMMUNICATION_PORT);
            client.send();
        } else {
            std::cout << "Unknown option\n";
        }
    }    
    catch(const std::exception &ex){
        std::cout << ex.what();
    }
    
    return 0;
}