#pragma once

#include <functional>

class MessageHandler{
    private:
        std::function<void(std::string, std::string)> send_function_ = nullptr;
        std::string waiting_for_response_addr_;
        bool exit_after_response_receive_ = false;
    public:
        //this function should be commented in order to make initialization
        //  of std function work
        //void handleMessage(std::string message);
        void handleMessage(std::string ip_address, std::string message);
        void queueMessage(std::string ip_address, std::string message);
        void setSendFunction(std::function<void(std::string, std::string)> func);
        void setExitAfterResponseReceive(bool state){exit_after_response_receive_ = state;};
};