#include <iostream>

#include "include/message.hpp"

/*
void MessageHandler::handle_message(std::string message){
    std::cout << "[MESSAGE HANDLER] " << message << "\n";
}
*/
void MessageHandler::handle_message(std::string ip_address,
    std::string message){
    
    std::cout << "[MESSAGE HANDLER] " << message << "\n";
}