#include <string>
#include <iostream>
#include <functional>
#include <thread>

#include "communication.hpp"
#include "logger.hpp"
#include "message.hpp"
#include "types.hpp"

#define DEFAULT_PORT  (6300)

#define PROGRAM_MODE_ANSWERING      (0)
#define PROGRAM_MODE_SINGLE_SEND    (1)

static unsigned program_mode;
static std::string default_ip_address;

bool static _parse_input_arguments(int argc, char* argv[]){
    switch(argc){
        case 1:
            _LOG(ERROR) << "Not enough input arguments";
            break;
        case 2:
            if(!std::string("answering").compare(std::string(argv[1]))){
                program_mode = PROGRAM_MODE_ANSWERING;
                return 0;
            }
            _LOG(ERROR) << "Invalid input arguments";
            break;
        case 3:
            if(!std::string("single").compare(std::string(argv[1]))){
                program_mode = PROGRAM_MODE_SINGLE_SEND;
                default_ip_address = std::string(argv[2]);
                return 0;
            }
            _LOG(ERROR) << "Invalid input arguments";
            break;
        default:
            _LOG(ERROR) << "Too many input arguments";
            break;
    }  
    return 1;
}

int main(int argc, char* argv[]){
    
    logger.set_logging_level(INFO);

    if(_parse_input_arguments(argc, argv))
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

        if(program_mode == PROGRAM_MODE_ANSWERING){
        } else if(program_mode == PROGRAM_MODE_SINGLE_SEND){
            msg_handler.setWaitForResponse(true);
            msg_handler.queueMessage(default_ip_address, "default_message");
            msg_handler.queueMessage(default_ip_address, "default_message_2");
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