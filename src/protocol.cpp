#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "include/protocol.hpp"
#include "include/logger.hpp"
#include "include/types.hpp"

void MessageHandler::parseMessageType(std::string &payload, protocol_msg_t &msg){
    std::string delimiter = " ";
    size_t pos = payload.find(delimiter);
    std::string token = payload.substr(0, pos);
    payload.erase(0, pos + delimiter.length());
    msg.type = static_cast<protocol_msg_type>(stoi(token));
}

void MessageHandler::handleMessage(std::string ip_address,
    std::string message){
    
    protocol_msg_t msg;
    protocol_msg_t msg_response;
    
    parseMessageType(message, msg);
    
    switch(msg.type){
        case DISCOVERY:
            msg_response.type = DISCOVERY_ACK;
            queueMessage(ip_address, msg_response);
            break;
        case DISCOVERY_ACK:
            _LOG(INFO) << "received DISCOVERY_ACK " + ip_address;
            break;
        default:
            throw(std::runtime_error("invalid message type"));
    }
}

void MessageHandler::setSendFunction(
    std::function<void(std::string, std::string)> func){
        send_function_ = func;
}

void MessageHandler::queueMessage(std::string ip_address,
    protocol_msg_t message){
    
    send_function_(ip_address, message.toString());
    _LOG(DEBUG) << "queued up message";
}