#include <netinet/in.h>
#include <string>

#define RECV_MESSAGE_BUFFER_LEN (100)
#define DEFAULT_COMMUNICATION_PORT  (IPPORT_USERRESERVED + 1300) //6300
#define SERVER_CONNECTION_REQUEST_QUEUE    (1)

class UdpServer{
    private:
        int _fd;
        sockaddr_in _addr_in;
        uint8_t _recv_message_buffer[RECV_MESSAGE_BUFFER_LEN];

    public:
        UdpServer();
        void run();
};

class UdpClient{
    private:
        int _fd_dst;
        sockaddr_in _addr_out;
        std::string _input;
    
    public:
        UdpClient();
        void run();
};