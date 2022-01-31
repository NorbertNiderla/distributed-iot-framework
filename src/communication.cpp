#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>

#include "include/communication.hpp"
#include "include/logger.hpp"

Socket::Socket(int type){
    if(type == PROTOCOL_UDP)
        file_descriptor_ = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
    else if(type == PROTOCOL_TCP)
        file_descriptor_ = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
    
    if(file_descriptor_ == -1){
        throw(std::runtime_error("Opening socket failed"));
    }
}

InetSocketAddress::InetSocketAddress(uint32_t port){
    // sets IpAdress to INADDR_ANY
    address_ = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
}

void InetSocketAddress::setIpAddress(std::string ip_address){
    int ret = inet_pton(AF_INET, ip_address.c_str(), &address_.sin_addr.s_addr);
    //inet_pton() already converts address into net byte order
    if(ret != 1){
        throw(std::invalid_argument("Invalid IpAddress"));
    }
}

InetSocketAddress::InetSocketAddress(uint32_t port, std::string ip_address){
    address_ = {.sin_family = AF_INET, .sin_port = htons(port),
        .sin_addr = {.s_addr = htonl(INADDR_ANY)}};
    setIpAddress(ip_address);
}

UdpCommunicationHandler::UdpCommunicationHandler(uint32_t port,
    std::function<void(std::string, std::string)> receive_callback)
        : socket_(PROTOCOL_UDP), receive_callback_(receive_callback), port_(port){}

void UdpCommunicationHandler::send(std::string ip_addr, std::string message){
    sending_queue_.pushToSend(ip_addr, message);
}


static void _setNonBlocking(int fd)
{
    int flags;
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if(ret == -1){
        throw(std::runtime_error("Could not set socket to nonblocking"));
    }
}     

void UdpCommunicationHandler::run(){
    InetSocketAddress listen_addr(port_);
    int bind_ret = bind(socket_.getFd(), listen_addr.getAddrPtr(), 
        listen_addr.getAddrSize());
    
    if(bind_ret == -1)
        throw std::runtime_error("Bind failed");

    _setNonBlocking(socket_.getFd());

    socklen_t incoming_addr_size = listen_addr.getAddrSize();
    InetSocketAddress send_addr(port_);
    std::string msg;
    std::string ip;

    while(true){
        int recv_ret = recvfrom(socket_.getFd(), buffer_.data(),
            buffer_.max_size(),MSG_WAITALL, listen_addr.getAddrPtr(),

            &incoming_addr_size);

        if(recv_ret != -1){ 
            //received something
            _LOG(DEBUG) << "received something";

            char ip_address_ch[INET_ADDRSTRLEN];
            
            const char* conv_ret = inet_ntop(AF_INET,
                listen_addr.getSAddrPtr(),ip_address_ch, INET_ADDRSTRLEN);
            
            if(conv_ret == nullptr){
                throw(std::runtime_error("Conversion of IpAddress to string interpretation failed"));
            }
            //here should be a way to restart UdpHandler::run(),
            //  maybe run() should be in wrapper to catch all exceptions?

            _LOG(INFO) << std::string("received: ") + std::string(buffer_.data());

            receive_callback_(std::string(ip_address_ch), 
                std::string(buffer_.data()));   
        }
        //here should be done error checking for recvfrom

        //check for any messages to send
        if(sending_queue_.getToSend(ip, msg) != 0){

            send_addr.setIpAddress(ip);

            int send_ret = sendto(socket_.getFd(),
                (void*)msg.data(), msg.length(), 0,

            send_addr.getAddrPtr(), send_addr.getAddrSize());

            if(send_ret == -1){
                throw(std::runtime_error("Sending failed"));
            }
    
            _LOG(DEBUG) << "message sent";
        }
    }
}

int SendingQueue::getToSend(std::string &ip, std::string &msg){
    mtx_.lock();
    int ret = send_msg_q_.size();
    if(ret != 0){
        ip = send_ip_q_.front();
        msg = send_msg_q_.front();
        send_ip_q_.pop();
        send_msg_q_.pop();
    }
    mtx_.unlock();
    return ret;
}

void SendingQueue::pushToSend(std::string ip, std::string msg){
    mtx_.lock();
    send_ip_q_.push(ip);
    send_msg_q_.push(msg);
    mtx_.unlock();
}


TcpCommunicationHandler::TcpCommunicationHandler(uint32_t port,
    std::function<void(std::string, std::string)> accept_callback)
    :socket_(PROTOCOL_TCP), port_(port), accept_callback_(accept_callback){

    thd_ = std::thread(std::bind(TcpCommunicationHandler::run), this);    
}

void TcpCommunicationHandler::run(){
    InetSocketAddress listen_addr(port_);
    int bind_ret = bind(socket_.getFd(), listen_addr.getAddrPtr(), 
        listen_addr.getAddrSize());
    
    if(bind_ret == -1)
        throw std::runtime_error("Bind failed");

    _setNonBlocking(socket_.getFd());

    int listen_ret = listen(socket_.getFd(), MAX_TCP_CONNECTIONS);

    if(listen_ret == -1)
        throw(std::runtime_error("listen init failed"));
    
    socklen_t listen_addr_size = listen_addr.getAddrSize();
    
    while(true){
        int accept_ret = accept(socket_.getFd(), listen_addr.getAddrPtr(),
            &listen_addr_size);
        
        if(accept_ret != -1){        
            //here should be done checking of error type
            char ip_address_ch[INET_ADDRSTRLEN];
            
            const char* conv_ret = inet_ntop(AF_INET,
                listen_addr.getSAddrPtr(),ip_address_ch, INET_ADDRSTRLEN);
            
            if(conv_ret == nullptr){
                throw(std::runtime_error("Conversion of IpAddress to string interpretation failed"));
            }

            std::string ip(conv_ret);

            _setNonBlocking(accept_ret);
            sendAcceptResponse(accept_ret, ip);
            addConnection(accept_ret, ip);
        }

        checkConnections();
    }
}

void TcpCommunicationHandler::addConnection(int fd, std::string ip){
    TcpConnectionHandler tcp_conn(fd);
    connections_.push_back(tcp_conn);
}

void TcpCommunicationHandler::checkConnections(){
    //here we are receiving and sending from every connections we
    // have in connections_ vector
    InetSocketAddress listen_addr(port_);
    socklen_t incoming_addr_size = listen_addr.getAddrSize();
    std::string msg;
    std::string ip;
    
    for(auto &conn : connections_){
        int recv_ret = recvfrom(conn.getFd(), conn.getBufferData(),
            conn.getBufferMaxSize(),MSG_WAITALL, listen_addr.getAddrPtr(),
            &incoming_addr_size);

        if(recv_ret != -1){ 
            //received something
            _LOG(DEBUG) << "received something";

            char ip_address_ch[INET_ADDRSTRLEN];
            
            const char* conv_ret = inet_ntop(AF_INET,
                listen_addr.getSAddrPtr(),ip_address_ch, INET_ADDRSTRLEN);
            
            if(conv_ret == nullptr){
                throw(std::runtime_error("Conversion of IpAddress to string interpretation failed"));
            }
            //here should be a way to restart UdpHandler::run(),
            //  maybe run() should be in wrapper to catch all exceptions?

            _LOG(INFO) << std::string("received: ") + std::string(conn.getBufferData());

            receive_callback_(std::string(ip_address_ch), 
                std::string(conn.getBufferData()));   
        }
        //here should be done error checking for recvfrom

        //check for any messages to send
        if(conn.getToSend(ip, msg) != 0){

            InetSocketAddress send_addr(port_, ip);

            int send_ret = sendto(conn.getFd(),
                (void*)msg.data(), msg.length(), 0,

            send_addr.getAddrPtr(), send_addr.getAddrSize());

            if(send_ret == -1){
                throw(std::runtime_error("Sending failed"));
            }
    
            _LOG(DEBUG) << "message sent";
        }
    }
}

void TcpCommunicationHandler::sendAcceptResponse(int fd, std::string ip_addr){
    
    InetSocketAddress send_addr(port_, ip_addr);
    std::string msg("hello");
    int send_ret = sendto(fd, (void*)msg.data(), msg.length(), 0,
        send_addr.getAddrPtr(), send_addr.getAddrSize());

    if(send_ret == -1){
        throw(std::runtime_error("Sending failed"));
    }
    
    _LOG(DEBUG) << "accept response sent";
}


