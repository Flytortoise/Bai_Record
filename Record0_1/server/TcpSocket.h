#ifndef TCPSOCKET
#define TCPSOCKET

#include <netinet/in.h>

class TcpSocket
{
public:
    TcpSocket(int port, const char *ip);
	~TcpSocket();
    int start();

private:
	int m_server_socket;
	int m_accept_socket;
    socklen_t m_socketlen;
    struct sockaddr_in m_server_addr;
    struct sockaddr_in m_accept_addr;
};




#endif
