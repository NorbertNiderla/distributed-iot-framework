#pragma once
#include <cstdint>
#include <string>

class IPv4{
    private:
        uint8_t address[4];

    public:
        IPv4(){
            address[0] = 0;
            address[1] = 0;
            address[2] = 0;
            address[3] = 0;
        }

        IPv4(std::string ipv4_string){
            size_t pos;
            int i = 0;
            while((pos = ipv4_string.find(".")) != std::string::npos){
                std::string token = ipv4_string.substr(0, pos);
                ipv4_string.erase(0, pos + std::string(".").length());
                address[i++] = std::stoi(token);
            }
            address[i] = std::stoi(ipv4_string);
        }

        //converts uint32_t representation of ipv4 in network byte order into 
        //uint8_t buffer in standard order
        IPv4(uint32_t ip){
            readFromNetByteOrder(ip);
        }

        std::string toString() const{
            std::string ipv4_string = std::to_string(address[0]) + "."
                + std::to_string(address[1]) + "." 
                + std::to_string(address[2]) + "." 
                + std::to_string(address[3]);
            return ipv4_string;
        }

        inline const uint32_t toNetByteOrder() const noexcept{
            uint32_t ip = 0;
            ip |= (uint32_t)address[3] << 24;
            ip |= (uint32_t)address[2] << 16;
            ip |= (uint32_t)address[1] << 8;
            ip |= (uint32_t)address[0];

            return ip;
        }

        inline const void readFromNetByteOrder(uint32_t ip) noexcept{
            address[0] = ip & 0xFF;
            address[1] = (ip >> 8) & 0xFF;
            address[2] = (ip >> 16) & 0xFF;
            address[3] = (ip >> 24) & 0xFF;
        }
};
