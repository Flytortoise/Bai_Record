#include "TcpSocket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

TcpSocket::TcpSocket(int port, const char *ip)
{
    m_socketlen = sizeof(struct sockaddr);
    m_server_socket = socket(AF_INET, SOCK_STREAM,0);
    if(m_server_socket < 0)
    {
    	perror("Serve_sock error!");
		exit(-1);
    }

    int on = 1;
    if(setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR,&on, sizeof(on)) <0)
    {
        perror("Setsockopt error !");
        exit(-1);
    }

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(port);
    m_server_addr.sin_addr.s_addr = inet_addr(ip);
    if(bind(m_server_socket,(struct sockaddr *)&m_server_addr, m_socketlen) < 0)
    {
        perror("Bind error!");
        exit(-1);
    }

    if(listen(m_server_socket, 5) < 0)
    {
        perror("Listen error !");
        exit(-1);
    }
}

TcpSocket::~TcpSocket()
{

}

int TcpSocket::getClient()
{ 
    bzero(&m_accept_addr, sizeof(m_accept_addr));
    if((m_accept_socket = accept(m_server_socket, (struct sockaddr *)&m_accept_addr, &m_socketlen)) < 0)
    {
        perror("Accept error !");
        exit(-1);
    }

    return m_accept_socket;
}
