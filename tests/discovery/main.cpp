#include <string>
#include <iostream>
#include <functional>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <thread>
#include <chrono>

#include "communication.hpp"
#include "logger.hpp"
#include "protocol.hpp"
#include "types.hpp"

#define DEFAULT_PORT  (6300)
#define PROGRAM_MODE_DEFAULT 0
#define PROGRAM_MODE_DISCOVERY 1

static int program_mode = PROGRAM_MODE_DEFAULT;
static std::vector<std::string> discovery_ip_address;
static std::vector<std::thread> threads;
static int timeout_exit_seconds = 0;

bool static parseInputArguments(int argc, char* argv[]){
    int c;
    opterr = 0;
    std::string ip_str;
    std::string ip_delimiter(",");
    size_t pos;

    while((c = getopt(argc, argv, "e:d:")) != -1){
        switch(c)
        {
            case 'e':
                timeout_exit_seconds = std::stoi(std::string(optarg));
                break;
            case 'd':
                program_mode = PROGRAM_MODE_DISCOVERY;
                ip_str = std::string(optarg);
                while((pos = ip_str.find(ip_delimiter)) != std::string::npos){
                    std::string token = ip_str.substr(0, pos);
                    ip_str.erase(0, pos + ip_delimiter.length());
                    discovery_ip_address.push_back(token);
                }
                discovery_ip_address.push_back(ip_str);
                break;
            default:
                break;
        }
    }

    for(int index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    
    return 0;
}

int main(int argc, char* argv[]){
    
    logger.set_logging_level(INFO);
    if(parseInputArguments(argc, argv))
       return 1;
    _LOG(INFO) << "starting app";
    
    try{
        MessageHandler msg_handler;
        
        std::function<void(std::string,std::string)> cb_rx = 
            std::bind(&MessageHandler::handleMessage, &msg_handler,
                std::placeholders::_1, std::placeholders::_2);
        
        UdpCommunicationHandler udp_handler(DEFAULT_PORT, cb_rx);
        
        std::function<void(std::string,std::string)> cb_tx = 
            std::bind(&UdpCommunicationHandler::send,
                &udp_handler, std::placeholders::_1, std::placeholders::_2);
        
        msg_handler.setSendFunction(cb_tx);
        
        std::thread thread_udp_communication(
            std::bind(&UdpCommunicationHandler::run, &udp_handler));

        
        if(program_mode == PROGRAM_MODE_DEFAULT){
        } else if(program_mode == PROGRAM_MODE_DISCOVERY){
            protocol_msg_t msg = {.type = DISCOVERY};
            for(auto &ip : discovery_ip_address)
                msg_handler.queueMessage(ip, msg);
        }
        
        if(timeout_exit_seconds != 0){
            std::this_thread::sleep_for(std::chrono::seconds(timeout_exit_seconds));
            udp_handler.stopRunning();
        }

        thread_udp_communication.join();
    }
    catch(const NeedToExitException &ex){
        _LOG(INFO) << ex.what();
    }    
    catch(const std::exception &ex){
        _LOG(WARNING) << ex.what();
    }
    
    return 0;
}