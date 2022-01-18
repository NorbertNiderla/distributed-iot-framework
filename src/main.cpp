#include <string>
#include <iostream>
#include "include/udp_client.hpp"

int main(int argc, char* argv[]){
    if(argc != 2){
        return 1;
    }

    if(!std::string("receive").compare(std::string(argv[1]))){
        UdpClient client;
        try{
            client.receive_run();
        }
        catch(const std::exception &ex){
            std::cerr << ex.what();
        }
    } else if(!std::string("send").compare(std::string(argv[1]))) {
        UdpClient client;
        try{
            client.send();
        }
        catch(const std::exception &ex){
            std::cerr << ex.what();
        }
    } else {
        std::cout << "Unknown option\n";
    }
    
    return 0;
}