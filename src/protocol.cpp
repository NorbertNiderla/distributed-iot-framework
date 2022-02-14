#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "include/protocol.hpp"
#include "include/logger.hpp"
#include "include/types.hpp"

void MessageHandler::addKnownNode(IPv4 ip){
    known_nodes_.push_back(ip);
    _LOG(DEBUG) << "new known node " + ip.toString();
}

void MessageHandler::parseMessageType(std::string &payload, protocol_msg_t &msg){
    std::string delimiter = " ";
    size_t pos = payload.find(delimiter);
    std::string token = payload.substr(0, pos);
    payload.erase(0, pos + delimiter.length());
    msg.type = static_cast<protocol_msg_type_t>(stoi(token));
}

void MessageHandler::parseMessageNewNodes(std::string payload){
    std::string delimiter = " ";
    size_t pos;
    while((pos = payload.find(delimiter)) != std::string::npos){
        std::string token = payload.substr(0, pos);
        payload.erase(0, pos + delimiter.length());
        addKnownNode(token);
    }
}

void MessageHandler::handleMessage(IPv4 ip, std::string message){
    
    protocol_msg_t msg;
    protocol_msg_t msg_response;
    std::string known_nodes = "";
    
    parseMessageType(message, msg);
    
    switch(msg.type){
        case DISCOVERY:
            msg_response.type = DISCOVERY_ACK;
            queueMessage(ip, msg_response);
            break;
        case DISCOVERY_ACK:
            _LOG(INFO) << "received DISCOVERY_ACK " + ip.toString();
            break;
        case GET_KNOWN_NODES:
            for(auto &ip_addr : known_nodes_)
                known_nodes = known_nodes + ip.toString() + " ";
            msg_response.type = GET_KNOWN_NODES_RESPONSE;
            msg_response.data = known_nodes;
            queueMessage(ip, msg_response);
            addKnownNode(ip);
            break;
        case GET_KNOWN_NODES_RESPONSE:
            parseMessageNewNodes(message);
            break;
        default:
            throw(std::runtime_error("invalid message type"));
    }
}

void MessageHandler::setSendFunction(
    std::function<void(IPv4, std::string)> func){
        send_function_ = func;
}

void MessageHandler::queueMessage(IPv4 ip,
    protocol_msg_t message){
    
    send_function_(ip, message.toString());
    _LOG(DEBUG) << "queued up message";
}