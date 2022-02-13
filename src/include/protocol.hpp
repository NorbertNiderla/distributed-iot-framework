#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <string>

#include "logger.hpp"

typedef enum{
    DISCOVERY = 0,
    DISCOVERY_ACK,
    GET_KNOWN_NODES,
    GET_KNOWN_NODES_RESPONSE
}protocol_msg_type_t;

typedef struct{
    protocol_msg_type_t type;
    std::string data;

    std::string toString() const{
        return std::to_string(type) + " " +data;
    }
}protocol_msg_t;

class MessageHandler{
    private:
        std::function<void(std::string, std::string)> send_function_ = nullptr;
        std::vector<std::string> known_nodes_;
    public:
        MessageHandler(){};
        void handleMessage(std::string ip_address, std::string message);
        void queueMessage(std::string ip_address, protocol_msg_t msg);
        void setSendFunction(std::function<void(std::string, std::string)> func);
        void parseMessageType(std::string &payload, protocol_msg_t &msg);
        void parseMessageNewNodes(std::string payload);
        void addKnownNode(std::string ip_addr);
};