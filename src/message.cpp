#include <iostream>

#include "include/message.hpp"
#include "include/logger.hpp"
#include "include/types.hpp"

void MessageHandler::handleMessage(std::string ip_address,
    std::string message){
    
    _LOG(DEBUG) << "handling message";
    
    if(!ip_address.compare(waiting_for_response_addr_)){
        // if waiting for response go here
        std::cout << "[MESSAGE HANDLER] " << message << "\n";
        if(exit_after_response_receive_ == true)
            throw(NeedToExitException()); //here should be throwed my own exception type
    } else {
        // if we are not waiting for response then we are responding
        queueMessage(ip_address, "response");
    } 
}

void MessageHandler::setSendFunction(
    std::function<void(std::string, std::string)> func){
        send_function_ = func;
}

void MessageHandler::queueMessage(
    std::string ip_address,std::string message){
    waiting_for_response_addr_ = ip_address;
    send_function_(ip_address, message);
    _LOG(DEBUG) << "queued up message";
}