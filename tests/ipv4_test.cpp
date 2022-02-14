#include "../src/include/ip_v4.hpp"
#include <string>
#include <iostream>

int main(){
    IPv4 a;
    IPv4 b(std::string("192.168.12.1"));
    IPv4 c((uint32_t)0x010ca8c0);

    if(a.toString().compare("0.0.0.0") != 0){
        std::cout << "a.toString() failed\n";
        return 1;
    }
    if(b.toString().compare("192.168.12.1") != 0){
        std::cout << "b.toString() failed\n";
        return 1;
    }
    if(c.toString().compare("192.168.12.1") != 0){
        std::cout << "c.toString() failed\n";
        return 1;
    }
        
    if(a.toNetByteOrder() != 0){
        std::cout << "a.toNetByteOrder() failed\n";
        return 1;
    }
    if(b.toNetByteOrder() != 0x010CA8C0){
        std::cout << "b.toNetByteOrder() failed\n";
        return 1;
    }
    if(c.toNetByteOrder() != 0x010CA8C0){
        std::cout << "c.toNetByteOrder() failed\n";
        return 1;
    }
    
    return 0;
}