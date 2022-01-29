#pragma once

#include <functional>
#include <vector>
#include <mutex>

class MessageHandler{
    private:
        std::function<void(std::string, std::string)> send_function_ = nullptr;
        std::vector<std::string> waiting_for_response_;
        bool exit_after_response_receive_ = false;
        bool wait_for_response_ = false;
        std::mutex mtx_;
    public:
        void handleMessage(std::string ip_address, std::string message);
        void queueMessage(std::string ip_address, std::string message);
        void setSendFunction(std::function<void(std::string, std::string)> func);
        void setExitAfterResponseReceive(bool state){exit_after_response_receive_ = state;};
        void setWaitForResponse(bool state){wait_for_response_ = state;};
};