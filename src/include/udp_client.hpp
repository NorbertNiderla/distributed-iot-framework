#include <netinet/in.h>
#include <array>

#define DEFAULT_COMMUNICATION_PORT  (IPPORT_USERRESERVED + 1300) //6300
#define SERVER_CONNECTION_REQUEST_QUEUE    (1)
#define MESSAGE_BUFFER_LEN (100)

class UdpClient{
    private:
        int _fd;
        sockaddr_in _addr;
        std::array<char, MESSAGE_BUFFER_LEN> _buffer = {};
    
    public:
        UdpClient();
        void send();
        void receive_run();
};