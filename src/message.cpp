#include <iostream>
#include <vector>
#include <algorithm>

#include "include/message.hpp"
#include "include/logger.hpp"
#include "include/types.hpp"

void MessageHandler::handleMessage(std::string ip_address,
    std::string message){
    
    _LOG(DEBUG) << "handling message";

    mtx_.lock();
    std::vector<std::string>::iterator it = std::find(waiting_for_response_.begin(),
        waiting_for_response_.end(), ip_address);
    
    if(it != waiting_for_response_.end()){
        waiting_for_response_.erase(it);
        mtx_.unlock();
        std::cout << "[MESSAGE HANDLER] " << message << "\n";
        if(exit_after_response_receive_ == true)
            throw(NeedToExitException());
    } else {
        mtx_.unlock();
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
    
    if(wait_for_response_){
        mtx_.lock();
        waiting_for_response_.push_back(ip_address);
        mtx_.unlock();
    }
    
    send_function_(ip_address, message);
    _LOG(DEBUG) << "queued up message";
}