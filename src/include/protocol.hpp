#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <string>

#include "logger.hpp"

typedef enum{
    DISCOVERY = 0,
    DISCOVERY_ACK
}protocol_msg_type;

typedef struct{
    protocol_msg_type type;

    std::string toString() const{
        return std::string(std::to_string(type));
    }
}protocol_msg_t;

class MessageHandler{
    private:
        std::function<void(std::string, std::string)> send_function_ = nullptr;
    public:
        MessageHandler(){};
        void handleMessage(std::string ip_address, std::string message);
        void queueMessage(std::string ip_address, protocol_msg_t msg);
        void setSendFunction(std::function<void(std::string, std::string)> func);
        void parseMessageType(std::string &payload, protocol_msg_t &msg);
};