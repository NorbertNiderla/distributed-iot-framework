#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <string>

#include "logger.hpp"
#include "ip_v4.hpp"

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
        std::function<void(IPv4, std::string)> send_function_ = nullptr;
        std::vector<IPv4> known_nodes_;
    public:
        MessageHandler(){};
        void handleMessage(IPv4 ip, std::string message);
        void queueMessage(IPv4 ip, protocol_msg_t msg);
        void setSendFunction(std::function<void(IPv4, std::string)> func);
        void parseMessageType(std::string &payload, protocol_msg_t &msg);
        void parseMessageNewNodes(std::string payload);
        void addKnownNode(IPv4 ip);
};