#pragma once

#include "include/udp_comm.hpp"

class MessageHandler{
    private:
        std::function<void(std::string, string_ptr_t message)> send_function_;
    public:
        //this function should be commented in order to make initialization
        //  of std function work
        //void handle_message(std::string message);
        void handle_message(std::string ip_address, std::string message);
        void queue_message(std::string ip_address, std::string message);
};